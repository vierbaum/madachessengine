##
# Project Title
#
# @file
# @version 0.1

compile:
	$(CC) -O3 -funroll-loops -march=native -c *\.c
	$(CC) *\.o -o madaChess -O3 -march=native
	# $(CC) -c *\.c
	# $(CC) *\.o -o
	rm *\.o

debug:
	$(CC) -c -g *\.c
	$(CC) *\.o -o madaChess -g
	rm *\.o

# end
