#!/usr/bin/python

import re



pattern_poi = '^.+21.03.2020 (\d\d:\d\d:\d\d[.]\d\d\d).+saveIconIDs.+tPOIList[.]size.+:\s+(\d+)\s*$'
pattern_memory = '^.+21.03.2020 (\d\d:\d\d:\d\d[.]\d\d\d).+heartbeat:.+(\d+[.]\d+%)\s+(\d+[.]\d+%)\s+(\d+[.]\d+%)\s+(\d+)kB\s+(\d+)kB.+$'

def main( ):
   poi_count: int = 0

   file = open( "log.txt", "r" )
   # contents = file.read( )
   # print( contents )
   lines = file.readlines( )
   for line in lines:
      match = re.match( pattern_poi, line )
      if match:
         # print( match.group( 1, 2 ) )
         poi_count += int( match.group( 2 ) )

      match = re.match( pattern_memory, line )
      if match:
         time: str = match.group( 1 )
         memory: int = int( match.group( 6 ) )
         # print( '%s   %6d   %4d' % ( time, memory, poi_count ) )
         print( '%d' % ( poi_count ) )


   file.close( )



main( )
