ofLedsMapper
============

a leds mapper in OF

leds mapper made for a custom wall leds based on 25 modules (5*5), each one made of 8 * 4 pixels, each pixel made by 2 x 2 RGB leds
arduino files based on fast leds and receiving artnet threw ethernet shield

each module is a artnet universe, incrementing by 1 (IP and universe)
you can have x per y module, starting from top/left/bottom/right, wrapping in snake or not
each module has x and y sub-modules (any starting direction, wrapping)
each sub-modules has x and y RGB leds (any starting direction, wrapping)