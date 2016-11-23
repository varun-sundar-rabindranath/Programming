all:
	python setup.py  build_ext --inplace
	cp calculator.so libcalculator.so
	g++ -g -I/usr/include/python2.7 -I/usr/include/x86_64-linux-gnu/python2.7  -fno-strict-aliasing -D_FORTIFY_SOURCE=2 -g -fstack-protector-strong -Wformat -Werror=format-security  -DNDEBUG -g -fwrapv -O2 -Wall -Wstrict-prototypes -L/usr/lib/python2.7/config-x86_64-linux-gnu -L/usr/lib  -lpthread -ldl  -lutil -lm  -Xlinker -export-dynamic -Wl,-O1 -Wl,-Bsymbolic-functions -o driver driver.c -L. -lcalculator -lpython2.7
