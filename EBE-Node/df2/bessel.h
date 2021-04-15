#ifndef BESSEL_H_ 
#define BESSEL_H_


using namespace std;


class Bessel {
public:
	double I0(double x);
	double I1(double x);
	double K0(double x);
	double K1(double x);
	double Kn(int n, double x);

	// useful if K0 and K1 have already been computed!
	double Kn(int n, double K0, double K1, double x);
};

#endif


