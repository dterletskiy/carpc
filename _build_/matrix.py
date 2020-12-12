#!/usr/bin/python

import numpy as np
import scipy.special as sp

import base.console



def info( matrix: np.array ):
   base.console.debug.info( "dimentions: ", matrix.ndim )
   base.console.debug.info( "shape: ", matrix.shape )
# def info

def is_square( matrix: np.array ):
   for element in matrix.shape:
      if matrix.shape[0] != element:
         return False
   return True
# def is_square



def create_zeros( rows: int, columns: int ):
   return np.zeros( ( rows, columns ) )
# def create_zeros

def create_ones( rows: int, columns: int ):
   return np.ones( ( rows, columns ) )
# def create_ones

def create_random( rows: int, columns: int ):
   # return np.random.uniform( 0.001, 0.999, ( rows, columns ) )
   # return np.random.normal( 0.0, pow( columns, -0.5 ), ( rows, columns ) )
   return np.random.uniform( 0.0, pow( columns, -0.5 ), ( rows, columns ) )
# def create_random

def create( rows: int, columns: int, value = 1 ):
   return np.full( ( rows, columns ), value )
# def create



def create_diag( dimention: int, value = 1 ):
   return np.diag( np.diag( create( dimention, dimention, value ) ) )
# def create_diag

def create_diag_random( dimention: int ):
   return np.diag( np.diag( create_random( dimention, dimention ) ) )
# def create_diag_random

def make_diag( matrix: np.array ):
   if False == is_square( matrix ):
      raise MaxrixShapeError
   return matrix * create_diag( matrix.shape[0], 1 )
# def make_diag
