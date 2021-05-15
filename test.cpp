#include <iostream>

#include "EuropeanOption.hpp"
#include "GlobalFunction.hpp"
#include "OptionMatrix.hpp"
#include "PerpetualAmericanOption.hpp"
#include "AmericanOption.hpp"
using namespace std;

typedef EuropeanOption EO;
typedef PerpetualAmericanOption PAO;
typedef AmericanOption AO;

int main()
{
	
	//Create objects
	EO batch1c(60, 65, 0.25, 0.08, 0.30, 0.08, 1);
	EO batch1p(60, 65, 0.25, 0.08, 0.30, 0.08, 0);
	EO batch2c(100, 100, 1.0, 0.0, 0.2, 0.0, 1);
	EO batch2p(100, 100, 1.0, 0.0, 0.2, 0.0, 0);
	EO batch3c(5, 10, 1.0, 0.12, 0.50, 0.12, 1);
	EO batch3p(5, 10, 1.0, 0.12, 0.50, 0.12, 0);
	EO batch4c(100, 100, 30, 0.08, 0.3, 0.08, 1);
	EO batch4p(100, 100, 30, 0.08, 0.3, 0.08, 0);
	//Output the result
	cout << "Group A Exact solutions a): Test pricing function." << endl;
	cout << "Batch 1: C = " << batch1c.Price() << ", P = " << batch1p.Price() << endl;
	cout << "Batch 2: C = " << batch2c.Price() << ", P = " << batch2p.Price() << endl;
	cout << "Batch 3: C = " << batch3c.Price() << ", P = " << batch3p.Price() << endl;
	cout << "Batch 4: C = " << batch4c.Price() << ", P = " << batch4p.Price() << endl;

	cout << "\nGroup A Exact solutions b): Test put-call parity,version 1." << endl;
	cout << batch1c.Parity(60,2.13337) << endl;
	cout << batch2c.Parity(100, 7.96557) << endl;
	cout << batch3c.Parity(5, 0.204058) << endl;
	cout << batch4c.Parity(100, 92.17570) << endl;

	cout << "\nGroup A Exact solutions b): Test put-call parity,version 2." << endl;
	cout << batch1c.Parity(60, 2.13337, 5.84628) << endl;
	cout << batch2c.Parity(100, 7.96557, 7.96557) << endl;
	cout << batch3c.Parity(5, 0.204058, 4.07326) << endl;
	cout << batch4c.Parity(100, 92.17570, 1.24750) << endl;


	cout << "\nGroup A Exact solutions c): Test vector version of pricing function." << endl;
	vector<double> S1 = linspace(40, 80, 41); // Generate a vector
	for (unint i = 0;i < S1.size();i++)
		cout << batch1c.Price(S1)[i] << ", " ; // Call the vector pricing function


	
	cout << "\n\nGroup A Exact solutions d): Test matrix pricing function." << endl;
	vector<double> T1 = linspace(0.1, 0.4, 41);
	vector<double> sig1 = linspace(0.15, 0.45, 41);
	vector<double> r1 = linspace(0.04, 0.12, 41);
	OptionMatrix<EO> m1(41);
	for (unint i = 0; i < 41;i++) // Assign the matrix
	{
		m1(i + 1, 1) = S1[i];
		m1(i + 1, 2) = 65;
		m1(i + 1, 3) = T1[i];
		m1(i + 1, 4) = r1[i];
		m1(i + 1, 5) = sig1[i];
		m1(i + 1, 6) = r1[i];
	}
	for (unint i = 0; i < 41;i++) // Output
	{
		cout << i + 1 << " " << m1.CallPrice()[i] << ", ";
	}

	cout << "\n\nGroup A Greeks a): Test greeks function." << endl;
	EO forgreekscall(105, 100, 0.5, 0.1, 0.36, 0, 1);
	EO forgreeksput(105, 100, 0.5, 0.1, 0.36, 0, 0);
	cout << "Call delta: " << forgreekscall.Delta() << endl;
	cout << "Call gamma: " << forgreekscall.Gamma() << endl;
	cout << "Put delta: " << forgreeksput.Delta() << endl;
	cout << "Put gamma: " << forgreeksput.Gamma() << endl;

	cout << "\nGroup A Greeks b): Test greeks function, vector version." << endl;
	vector<double> S2 = linspace(85, 125, 41); // Generate a price vector
	for (unint i = 0;i < S2.size();i++) // Call the vector gamma function
		cout << i + 1 << " " << forgreekscall.Gamma(S2)[i] << ", "; 
	
	cout << "\n\nGroup A Greeks c): Test greeks function, matrix version." << endl;
	for (unint i = 0;i < S2.size();i++)
		cout << i + 1 << " " << m1.CallGamma()[i] << ", "; // Call the matrix gamma function

	cout << "\n\nGroup A Greeks d): Test approximate greeks." << endl;
	cout << "Call delta: " << forgreekscall.ApprDelta(0.0001) << endl; // Approximate, with h=0.0001
	cout << "Call gamma: " << forgreekscall.ApprGamma(0.0001) << endl;
	cout << "Put delta: " << forgreeksput.ApprDelta(0.0001) << endl;
	cout << "Put gamma: " << forgreeksput.ApprGamma(0.0001) << endl;

	cout << "\nGroup B b): Test pricing functions." << endl;
	PerpetualAmericanOption PAOcall(110, 100, 0.1, 0.1, 0.02, 1);
	PerpetualAmericanOption PAOput(110, 100, 0.1, 0.1, 0.02, 0);
	cout << "Call price: " << PAOcall.Price() << endl; // To call the pricing function
	cout << "Put price: " << PAOput.Price() << endl;

	cout << "\nGroup B c): Test pricing functions, vector version." << endl;
	vector<double> SB = linspace(100, 120, 21);
	for (unint i = 0;i < SB.size();i++)
		cout << i + 1 << ": " << PAOcall.Price(SB)[i]<<" "; // To call the vector pricing function

	cout << "\n\nGroup B d): Test pricing functions, matirx version." << endl;
	OptionMatrix<PAO> m2(21);
	vector<double> rB = linspace(0.05, 0.15, 21);
	vector<double> bB = linspace(0.01, 0.03, 21);
	for (unint i = 0; i < rB.size();i++)
	{
		m2(i + 1, 1) = SB[i];
		m2(i + 1, 2) = 100;
		m2(i + 1, 3) = 0;
		m2(i + 1, 4) = rB[i];
		m2(i + 1, 5) = 0.1;
		m2(i + 1, 6) = bB[i];
		cout << i + 1 << ": " << m2.CallPrice()[i] << " ";
	}

	cout << "\n\nTest MC,BT of AO" << endl;
	//EO batch2c(100, 100, 1.0, 0.0, 0.2, 0.0, 1);
	//EO batch2p(100, 100, 1.0, 0.0, 0.2, 0.0, 0);
	AO a2c(100, 100, 1, 0.1, 0.20, 0.1, 1);
	AO a2p(100, 100, 1, 0.1, 0.20, 0.1, 0);
	//cout << batch1c.Price() << endl;
	cout << a2c.MC(100, 1000, 1000) << endl; // 1000,289
	cout << a2c.BT(100, 2000) << endl;
	cout << a2p.MC(100, 1000, 1000) << endl; // 1000,289
	cout << a2p.BT(100, 2000) << endl;

	cout << "\n\nTest MC,FDM,BT of EO" << endl;
	//EO batch2c(100, 100, 1.0, 0.0, 0.2, 0.0, 1);
	//EO batch2p(100, 100, 1.0, 0.0, 0.2, 0.0, 0);
	cout << batch1c.Price() << endl;
	cout << batch1c.BT(60, 2000) << endl;
	cout << batch1c.FDM(60, 325,10000) << endl;
	cout << batch1c.MC(60,100000) << endl;

	/* // regression
	vector<vector<double>> x{ {2,2,3},{4,5,6},{2,2,1 },{10,11,12} };
	vector<double> b{ 1,2 ,3,4};
	for (auto& i : y.Regression(x,b)) { cout << i << endl; }
	*/
	//cout << batch1c.FDM(60, 325, 10000) << endl;
}