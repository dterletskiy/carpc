#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <cmath>

#include "api/sys/tools/Performance.hpp"
#include "api/sys/helpers/macros/strings.hpp"
#include "api/sys/math/TMatrix.hpp"
#include "test/other/nn/nn.hpp"

#include "api/sys/trace/Trace.hpp"
#define CLASS_ABBR "TEST:NN"



using tRow = base::math::tRow;
using tColumn = base::math::tColumn;
using tData = float;
using tMatrix = base::math::TMatrix< tData >;



auto sigma = [ ]( float& x, const tRow, const tColumn ){ x = 1.0 / ( 1.0 + exp(-x) ); };



class ILayer
{
   public:
      using tPtr = ILayer*;
      using tSptr = std::shared_ptr< ILayer >;
      using tWptr = std::weak_ptr< ILayer >;

   public:
      ILayer( ) { srand( time( nullptr ) ); }
      virtual ~ILayer( ) = default;

   public:
      virtual void feed_forward( const tMatrix& input_values ) = 0;
      virtual void feed_backward( const tMatrix& target_values ) = 0;
      virtual void print( ) const = 0;

   public:
      virtual const std::string& name( ) const = 0;
      virtual std::size_t neurons( ) const = 0;
      virtual const tMatrix& input( ) const = 0;
      virtual const tMatrix& output( ) const = 0;
      virtual const tMatrix& errors( ) const = 0;
      virtual const tMatrix& bias( ) const = 0;
      virtual const tMatrix& weights( ) const = 0;
      virtual ILayer* prev( ) const = 0;
      virtual void prev( ILayer* p_prev ) = 0;
      virtual ILayer* next( ) const = 0;
      virtual void next( ILayer* p_next ) = 0;
};

class Layer : public ILayer
{
   public:
      Layer( const std::string& name, const std::size_t neurons, ILayer* p_prev = nullptr );
      ~Layer( ) override;

   public:
      void feed_forward( const tMatrix& input_values ) override;
      void feed_backward( const tMatrix& target_values ) override;
      void print( ) const override;

   public:
      const std::string& name( ) const override;
      std::size_t neurons( ) const override;
      const tMatrix& input( ) const override;
      const tMatrix& output( ) const override;
      const tMatrix& errors( ) const override;
      const tMatrix& bias( ) const override;
      const tMatrix& weights( ) const override;
      ILayer* prev( ) const override;
      void prev( ILayer* p_prev ) override;
      ILayer* next( ) const override;
      void next( ILayer* p_next ) override;

   private:
      std::string m_name;
      std::size_t m_neurons = 1;

      ILayer* mp_prev = nullptr;
      ILayer* mp_next = nullptr;

      tMatrix m_output;
      tMatrix m_errors;
      tMatrix m_bias;
      tMatrix m_weights;
};



Layer::Layer( const std::string& name, const std::size_t neurons, ILayer* p_prev )
   : ILayer( )
   , m_name( name )
   , m_neurons( neurons )
   , mp_prev( p_prev )
   , m_output( tRow{ neurons }, tColumn{ 1 } )
   , m_errors( tRow{ neurons }, tColumn{ 1 } )
   , m_bias( tRow{ neurons }, tColumn{ 1 } )
   , m_weights( tRow{ neurons }, p_prev == nullptr ? tColumn{ neurons } : tColumn{ p_prev->neurons( ) } )
{
   if( nullptr == mp_prev )
   {
      m_bias.init( base::math::eCloneType::All, [ ]( const tRow, const tColumn ){ return 0.0f; } );
      m_weights.init( base::math::eCloneType::Diag, [ ]( const tRow, const tColumn ){ return 1.0f; } );
   }
   else
   {
      srand( time( nullptr ) );
      m_bias.init( base::math::eCloneType::All, [ ]( const tRow, const tColumn ){ return 0.1f; } );
      m_weights.init( base::math::eCloneType::All, [ ]( const tRow, const tColumn ){ return static_cast< float >( rand( ) ) / RAND_MAX; } );
      // m_weights.init( base::math::eCloneType::All, [ ]( const tRow, const tColumn ){ return 0.1f; } );
      mp_prev->next( this );
   }
}

Layer::~Layer( )
{
}

void Layer::feed_forward( const tMatrix& input_values )
{
   if( nullptr == mp_prev )
   {
      m_output = input_values;
   }
   else
   {
      m_output = m_weights.dot( mp_prev->output( ) ) + m_bias;
      m_output.apply( sigma );
   }
}

void Layer::feed_backward( const tMatrix& target_values )
{
   if( nullptr == mp_prev )
      return;

   if( nullptr == mp_next )
      m_errors = target_values - m_output;
   else
      m_errors = mp_next->weights( ).transpose( ).dot( mp_next->errors( ) );

   auto vector( m_output );
   vector.apply( [ ]( tData& x, const tRow, const tColumn ){ x = x * ( 1 - x ); } );
   vector *= m_errors;
   m_weights += 0.05 * vector.dot( mp_prev->output( ).transpose( ) );
   m_bias += 0.05 * vector;
}

void Layer::print( ) const
{
   SYS_INF( "Layer: %s(%p) / prev: %p / next: %p", m_name.c_str( ), this, mp_prev, mp_next );
   m_bias.print( "bias: " );
   m_weights.print( "weights: " );
   m_output.print( "outputs: " );
   m_errors.print( "errors: " );
}

const std::string& Layer::name( ) const
{
   return m_name;
}

std::size_t Layer::neurons( ) const
{
   return m_neurons;
}

const tMatrix& Layer::input( ) const
{
   return mp_prev->output( );
}

const tMatrix& Layer::output( ) const
{
   return m_output;
}

const tMatrix& Layer::errors( ) const
{
   return m_errors;
}

const tMatrix& Layer::bias( ) const
{
   return m_bias;
}

const tMatrix& Layer::weights( ) const
{
   return m_weights;
}

ILayer* Layer::prev( ) const
{
   return mp_prev;
}

void Layer::prev( ILayer* p_prev )
{
   mp_prev = p_prev;
}

ILayer* Layer::next( ) const
{
   return mp_next;
}

void Layer::next( ILayer* p_next )
{
   mp_next = p_next;
}



class NeuralNetwork
{
   public:
      using tLayers = std::vector< ILayer* >;
      using tDeviations = std::vector< tData >;

   public:
      NeuralNetwork( std::vector< std::size_t > layer_neurons );
      ~NeuralNetwork( );

   public:
      void print( ) const;
      const tMatrix& feed_forward( const tMatrix& input_values );
      void feed_backward( const tMatrix& target_values );
      void train( const tMatrix& input_values, const tMatrix& target_values, std::size_t print_statistics = 0 );

   public:
      const tLayers& layers( ) const;
      ILayer* layer( const std::size_t index ) const;

   private:
      tLayers     m_layers;
      std::size_t m_iterration = 0;
      tDeviations m_deviations = { };
};



NeuralNetwork::NeuralNetwork( std::vector< std::size_t > layer_neurons )
{
   if( 3 > layer_neurons.size( ) )
      throw std::invalid_argument( "Amount of layers should be more then 3" );

   ILayer* p_layer = new Layer( "input", layer_neurons[ 0 ], nullptr );
   m_layers.push_back( p_layer );
   for( std::size_t index = 1; index < layer_neurons.size( ) - 1; ++index )
   {
      p_layer = new Layer( base::format_string( "hidden_", index ), layer_neurons[ index ], p_layer );
      m_layers.push_back( p_layer );
   }
   p_layer = new Layer( "output", layer_neurons.back( ), p_layer );
   m_layers.push_back( p_layer );
}

NeuralNetwork::~NeuralNetwork( )
{
}

void NeuralNetwork::print( ) const
{
   SYS_INF( "Network:" );
   SYS_INF( "Train iterations: %zu", m_iterration );
   for( auto layer : m_layers )
      layer->print( );
}

const tMatrix& NeuralNetwork::feed_forward( const tMatrix& input_values )
{
   for( auto layer : m_layers )
      layer->feed_forward( input_values );

   return m_layers.back( )->output( );
}

void NeuralNetwork::feed_backward( const tMatrix& target_values )
{
   for( auto riterator = m_layers.rbegin( ); riterator != m_layers.rend( ); ++riterator )
      (*riterator)->feed_backward( target_values );
}

const NeuralNetwork::tLayers& NeuralNetwork::layers( ) const
{
   return m_layers;
}

void NeuralNetwork::train( const tMatrix& input_values, const tMatrix& target_values, std::size_t print_statistics )
{
   const tMatrix& output_values = feed_forward( input_values );
   feed_backward( target_values );
   ++m_iterration;

   tMatrix vector( output_values - target_values );
   auto sqr_action = [ ]( const tData& value, const tRow, const tColumn ){ return value * value; };
   tData a = vector.sum( sqr_action );
   tData b = target_values.sum( sqr_action );
   tData deviation = std::sqrt( a / b );
   m_deviations.push_back( deviation );

   if( 0 != print_statistics && 0 == m_iterration % print_statistics )
   {
      SYS_INF( "--------------- TRAIN STATISTICS ---------------" );
      SYS_INF( "iterations: %zu", m_iterration );
      input_values.print( "input:" );
      output_values.print( "output:" );
      target_values.print( "target:" );
      SYS_ERR( "deviation: %f", deviation );
      SYS_INF( "------------------------------------------------" );
   }
}

ILayer* NeuralNetwork::layer( const std::size_t index ) const
{
   if( m_layers.size( ) <= index )
   {
      std::string error = base::format_string( "index '", index, "' [ -", m_layers.size( ), ",", m_layers.size( ), ")" );
      throw std::invalid_argument( error.c_str( ) );
   }

   return m_layers[ index ];
}





namespace other::nn::test {

   bool run( )
   {
      SYS_ERR( "--------------- MARKER ---------------" );



      const std::size_t input_size = 3;
      NeuralNetwork nn( { input_size, 10, 10, 10, 10, 10, 1 } );

      srand( time( nullptr ) );
      auto generator = [ LO = -1.0, HI = 1.0 ]( const tRow row, const tColumn column )
      {
         return LO + static_cast< float > ( rand( ) ) / ( static_cast< float > ( RAND_MAX / ( HI - LO ) ) );
      };

      for( std::size_t iteration = 0; iteration < 10000000; ++iteration )
      {
         tMatrix input_values( tRow{ input_size }, tColumn{ 1 }, generator );
         const tData result = input_values.sum(
            [ ]( const tData& value, const tRow row, const tColumn column )
            {
               return pow( value, 2*row.value( ) );
            }
         );
         tMatrix target_values( tRow{ 1 }, tColumn{ 1 }, { result / input_size } );
         // input_values.print( "input values:" );
         // target_values.print( "target values:" );

         nn.train( input_values, target_values, 10000 );
      }
      


      return true;
   }

   bool run_h( )
   {
      SYS_ERR( "--------------- MARKER ---------------" );



      NeuralNetwork nn( { 1, 10, 10, 10, 10, 10, 1 } );
      // nn.print( );

      srand( time( nullptr ) );
      auto generator = [ LO = -1.0, HI = 1.0 ]( const tRow row, const tColumn column )
      {
         return LO + static_cast< float > ( rand( ) ) / ( static_cast< float > ( RAND_MAX / ( HI - LO ) ) );
         // return 0.1f;
      };

      for( std::size_t iteration = 0; iteration < 1000000; ++iteration )
      {
         // SYS_ERR( "--------------- GENERATED ---------------" );
         tMatrix input_values( tRow{ 1 }, tColumn{ 1 }, generator );
         tMatrix target_values( input_values );
         target_values.apply(
            [ ]( float& value, const tRow, const tColumn )
            {
               value *= value;
            }
         );
         // input_values.print( "input values:" );
         // target_values.print( "target values:" );

         nn.train( input_values, target_values, 10000 );
         // SYS_ERR( "--------------- FEED FORWARD ---------------" );
         // nn.feed_forward( input_values );
         // nn.print( );
         // SYS_ERR( "--------------- FEED BACKWARD ---------------" );
         // nn.feed_backward( target_values );
         // nn.print( );
         // SYS_ERR( "--------------- MARKER ---------------" );
      }
      


      return true;
   }

}