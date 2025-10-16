#include <iostream>

class NoSignature;

template < typename Generator >
struct TSignal;

template < typename T >
struct TSignalA
{
    void processA( )
    {
        std::cout << "processA( T )" << std::endl;
    }
};

template < >
struct TSignalA< NoSignature >
{
    void processA( )
    {
        std::cout << "processA( NoSignature )" << std::endl;
    }
};

template < typename T >
struct TSignalB
{
    void processB( )
    {
        std::cout << "processB( T )" << std::endl;
    }
};

template < >
struct TSignalB< NoSignature >
{
    void processB( int )
    {
        std::cout << "processB( NoSignature )" << std::endl;
    }
};

template < typename Generator >
struct TSignal
    : public TSignalA< typename Generator::tA >
    , public TSignalB< typename Generator::tB >
{
};

template < typename A, typename B >
struct TGenerator
{
    using tGenerator = TGenerator< A, B >;
    using tA = A;
    using tB = B;
    using tSignal = TSignal< tGenerator >;
};

int main( )
{
    {
        TSignal< TGenerator< int, int > > signal;
        signal.processA( );
        signal.processB( );
    }

    {
        TSignal< TGenerator< int, NoSignature > > signal;
        signal.processA( );
        signal.processB( 5 );
    }

    {
        TSignal< TGenerator< NoSignature, int > > signal;
        signal.processA( );
        signal.processB( );
    }

    {
        TSignal< TGenerator< NoSignature, NoSignature > > signal;
        signal.processA( );
        signal.processB( 5 );
    }

    return 0;
}
