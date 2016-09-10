COMPPARAMS = -m32

%.o: %.cpp
	g++ $(COMPPARAMS) -o $@ -c $<
