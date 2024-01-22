default:
	gcc main.c filehandler.c seclib.c -o sentsec -lssl -lcrypto
help:
	./sentsec help