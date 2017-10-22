/*  PROGRAM: całka
 *  WERSJA: 0.2
 *  Autor: Artur GRACKI mailto: arteq(at)arteq(dot)org
 *  OSTATNIA MODYFIKACJA: 2007/05/21 (pon) 13:53:27
 *  KEYWORDS: całka, całkowanie numeryczne, transformata falkowa
 */

#include	<iostream>
#include	<iomanip>
#include	<fstream>
#include	<math.h>

using namespace std;

const double pi = 3.141592654;
const double eps = 1e-5;

/* ====================================================================== */

double f(double x)
{
	return sin(20 * pi * x * x);
}

/* ====================================================================== */

int psi(double x, double a, double b)
{
	double argument;

	argument = (x - b) / a;
	if (argument > 0 && argument < 0.5) return 1;
	if (argument >= 0.5 && argument < 1) return -1;
	if (argument <= 0 || argument >= 1) return 0;
}

/* ====================================================================== */
// Liczenie metoda trapezow: przyblizenie obszaru podcalkowego trapezem
double trapez(double a, double b)
{
	double calka = 0.0;
	double calka_poprz = 0.0;
	double e, h;
	int n = 0;

	// Początkowe granice całkowania od 'b' do 'a+b'
	// poza tym przedziałem nie ma sensu liczyć, bo funkcja Psi i tak wszystko wyzeruje
	h = (a+b - b) / pow(2, n);
	calka_poprz = 0.5 * h * (f(a+b) + f(b));

	// Zaczynamy od podziału przedziału całkowania na 8 (2^3) kawałków
	// Jednocześnie 2^100 wydaje się być wystarczająco ciasny podziałem, 
	// żeby nigdy do niego nie doszło :-)
	for (n = 3; n < 100; n++)
	{
		h = (a+b - b) / pow(2, n);
		calka = 0.0;

		for (int iter = 0; iter < pow(2,n); iter++)
			calka += psi(b + iter*h, a, b) * 0.5 * h * ( f(b + iter*h) + f(b + (iter+1)*h));
		
		// Sprawdzamy o ile lepiej udało się policzyć całkę w stosunku do poprzedniej iteracji
		e = fabs(calka - calka_poprz); 

		// Robimy podmiankę dla ew. kolejnej iteracji
		calka_poprz = calka;

		// Wyrzucamy wyniki na ekran
//		cout << "N: " << n << "\th: " << h << "\tCalka: " << calka << "\n";
//		cout << "EPS: " << e << "\n" << endl;

		// Jeżeli uzyskaliśmy wystarczającą dokładność to kończymy pętle i zwracamy wynik
		if (e < eps) break;
	
	}

	// cout << "Uzyskano zadowalajaca dokladnosc w " << n << " iteracji" << endl;
	cout << "(" << n << ")\t";
	return 1 / sqrt(fabs(a)) * calka;
}

/* ====================================================================== */

// Liczenie metoda Simpsona: przyblizenie obszaru podcalkowego parabola
double simpson(double a, double b)
{
	double calka, calka_poprzednia = 0.0;
	double st, dx, x;
	int iter;

	// 100 iteracji powinno w zupełności wystarczyć :-)
	for (iter = 1; iter < 100; iter++)
	{
		double n = pow(2, iter);
		calka = 0.0;
		st = 0.0;
		 
		// Zakres calkowania: od 'b' do 'a+b'
		// Pewnie daloby sie jakos ten nawias jeszcze uproscic ;-)
		dx = (a+b - b) / n; 

		for (int i = 1; i <= n; i++)
		{
			x = b + i*dx;
			st += psi(x - dx/2.0, a, b) * f(x - dx/2.0);
			if (i < n) calka += psi(x, a, b) * f(x);
		}

		calka = dx / 6.0 * ( psi(b, a, b) * f(b) + psi(a+b, a, b) * f(a+b) + 2.0 * calka + 4.0 * st);
		
		// Sprawdzanie dokładności
		double e = fabs(calka_poprzednia - calka);
		calka_poprzednia = calka;
		
		// Jeżeli całka się zbytnio nie zmieniła to kończymy zabawę
		if ( e < eps) break;

	}

	cout << "(" << iter << ")\t";
	return 1 / sqrt(fabs(a)) * calka;
}
/* ====================================================================== */

int main()
{
	double a,b;
	double x;
	int j, k;
	char metoda;

	cout << "\n";
	cout << "//================================\n";
	cout << "// PROGRAM: caleczka\n";
	cout << "// AUTOR: Artur Gracki\n";
	cout << "// MAIL: arteq <at> arteq <dot> org\n";
	cout << "// DATA: " << __DATE__ << " " << __TIME__ << "\n";
	cout << "//================================\n\n";

	cout << "Jaka metoda bedziemy liczyli? \n";
	cout << " t - metoda trapezow\n";
	cout << " s - metoda Simpsona (lepsza)\n\n";

	cout << "Metoda: ";
	cin >> metoda;

	cout << "# caleczka \ta \t\tb \titer \tCalka\n" << endl;

	if (metoda == 't' || metoda == 's')
		for (j = 1; j < 7; j++)
		{
			for (k = 0; k < j; k++)
			{
				a = pow(2, -j);
				b = k * pow(2, -j);

				cout << "c (" << j << ", " << k << ")";
				cout << setw(10) << right << a << "\t";
				cout << setw(10) << right << b << "\t" << flush;

				if (metoda == 't')	cout << trapez(a , b) << endl; 
				if (metoda == 's')	cout << simpson(a , b) << endl; 
			}
			cout << "\n";
		}
	else	cout << "Naucz sie odpowiadac na pytania!!\n";

	return 0;
}
