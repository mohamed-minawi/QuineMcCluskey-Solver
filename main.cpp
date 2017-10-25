#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <bitset>
#include<iomanip>


using namespace std;

#include "Number.h"

int numofvar;
// Input Function
bool validateinput(vector<number> &, vector<number> &, int);						// Validate Input of minterms and don't cares
void inputmand(vector<number> &, vector<number> &, int &, int &);					// Input the minterms and don't care terms
// Implication Table Functions
void pushvecsort(vector<number> &, vector<number> &, vector<vector<number>> &);		// push the minterms and don't cares into first column
void createimptab(vector<vector<number>> &);										// Create the implication table	
void eliminate(vector<vector<number>> &);											// remove repeated terms in any column
void printimp(vector<vector<number>>&, vector<number> &);												// print implication table
// Coverage Table Functions
void createcovchart(vector<number>&, vector<number>&, vector<vector<number>> &);	// Create Coverage table
void printcov(vector<vector<number>> &);
void minimiziedexp(vector<vector<number>>, vector<number>&, vector<number>&, vector<int>&);
void printess(vector<vector<number>>&, vector<number>&, vector<int>&);

string binarytoalpha(string bin);

void main()
{
	int numofminterms, numofdontcares;
	vector<number> minterms, dontcares, primeimplicants, essprimeimp, min;		// vectors for the minterms, don't care terms, and prime implicants

	vector<vector<number>> implicationtable(16, vector<number>(0));		// Implication table with 16 empty cols
	vector<vector<number>> coveragetable(pow(2, 16), vector<number>(0));								// Coverage table
	vector<int> row;
	cout << "Please enter number of variables (0<=x<=16): ";
	cin >> numofvar;
	do
	{
		cout << "Please enter number of minterms: ";
		cin >> numofminterms;

		cout << "Please enter number of don't cares: ";
		cin >> numofdontcares;

	} while (numofminterms + numofdontcares > pow(2, numofvar));

	inputmand(minterms, dontcares, numofminterms, numofdontcares);		// Input of the minterms and don't cares

	pushvecsort(minterms, dontcares, implicationtable);					// Push the minterms and don't cares into first col of implication table and sort it according to number of ones
	createimptab(implicationtable);										// Create the implication table
	printimp(implicationtable, primeimplicants);						// Print the implication table and push the prime implicants into its vector.
	
	createcovchart(minterms, primeimplicants, coveragetable);
	printcov(coveragetable);
	printess(coveragetable, essprimeimp, row);
//	minimiziedexp(coveragetable, essprimeimp, min, row);

	system("pause");

}

string binarytoalpha(string bin)
{
	string alphabet[] = { "'" ,"A", "B", "C", "D","E","F","G","H","I", "J", "K", "L", "M", "N","O","P","Q","R","S","T","U","V","W","X","W","Y","Z", };
	string out = "";
	for (int i = 0; i < bin.size(); i++)
		if (bin[i] != '-')
		{
			if (bin[i] == '1')
				out = out + alphabet[i + 1];
			if (bin[i] == '0')
				out = out + alphabet[i + 1] + alphabet[0];

		}
	return out;
}

void inputmand(vector<number> & minterms, vector<number> &dontcares, int & numofminterms, int & numofdontcares)
{
	int input;
	bool renter = false, rentertext = false;
	if (numofminterms > 0)
		cout << "Please enter each minterm and press enter: " << endl;
	for (int i = 0; i < numofminterms; i++)
	{
		do
		{
			cin >> input;											// input of the minterm values
			if (!validateinput(minterms, dontcares, input))			// validate that it doesn't already exist or that it is within the acceptable rate
				renter = true, rentertext = true;
			else renter = false, rentertext = false;				// if input is incorrect, show this string and reneter input

			if (rentertext)
				cout << "Invalid. Please enter new value: " << endl;
		} while (renter);											// loop to make sure that the input is correct 
		number z;
		z.specifybit(numofvar);
		z.insert(input);										// push the input into the minterm vector if it is acceptable
		minterms.push_back(z);
	}
	if (numofdontcares > 0)
		cout << "Please enter each don''t care term and press enter: " << endl;

	for (int i = 0; i < numofdontcares; i++)
	{
		do
		{
			cin >> input;												// input of the don't care values
			if (!validateinput(minterms, dontcares, input))				// validate that it doesn't already exist or that it is within the acceptable rate
				renter = true, rentertext = true;
			else renter = false, rentertext = false;

			if (rentertext)
				cout << "Invalid. Please enter new value: " << endl;			// if input is incorrect, show this string and reneter input
		} while (renter);
		number z;
		z.specifybit(numofvar);
		z.insert(input);										// push the input into the minterm vector if it is acceptable
		dontcares.push_back(z);
	}

}
bool validateinput(vector<number> & minterms, vector<number> & dontcare, int input)
{
	if (input > pow(2, numofvar) - 1 || input<0)				// validate that the input is less than "2 to the power of the number of variables"
		return false;

	number z;
	z.specifybit(numofvar);
	z.insert(input);

	for (int i = 0; i < minterms.size(); i++)
		if (z.returnstring() == minterms[i].returnstring())			// Validate that the input doesn't already exist in the minterms 
			return false;


	for (int i = 0; i < dontcare.size(); i++)
		if (z.returnstring() == dontcare[i].returnstring())			// Validate that the input doesn't already exist in the don't cares 
			return false;

	return true;												// Returns true if the input is within the acceptable range and doesn't already exist
}

void pushvecsort(vector<number> & minterms, vector<number> &dontcares, vector<vector<number>> & implicationtable)
{
	number z;

	for (int i = 0; i < minterms.size(); i++)
		implicationtable[0].push_back(minterms[i]);				// push all the minterms into the first col of the implication table

	for (int i = 0; i < dontcares.size(); i++)
		implicationtable[0].push_back(dontcares[i]);			// push all the don't care terms into the first col of the implication table

	for (int i = 0; i < implicationtable[0].size() - 1; i++)
		for (int j = 0; j < implicationtable[0].size() - 1; j++)
			if (implicationtable[0][j].returnones() > implicationtable[0][j + 1].returnones())			// sort the first col of the implication according to the number of ones in the binary form
			{
				z = implicationtable[0][j];
				implicationtable[0][j] = implicationtable[0][j + 1];
				implicationtable[0][j + 1] = z;
			}


}
void createimptab(vector<vector<number>> & matrix)
{
	int count = 0, start = 0;
	bool end = false;						// because the maximum number of cols in the implication is 16, we need to work on the assumtpion that there is going to be 16 cols.
	string x;

	while (start < 16 && !end)				// while we still didn't reach the 16th col and didn't finish the program, the loop continues
	{
		end = true;
		for (int i = 0; i < matrix[start].size() - 1; i++)			// while accessing the values in the cols
		{
			for (int j = i + 1; j < matrix[start].size(); j++)			// while accessing the values in the cols
			{
				if (matrix[start][i].returnones() == matrix[start][j].returnones() - 1)		// if the difference between the number of ones of two numbers is 1 , we need to check the binary values
				{
					for (int k = 0; k < numofvar; k++)
						if (matrix[start][i].returnstring()[k] != matrix[start][j].returnstring()[k])
							count++;														// count the number of different bits
					if (count == 1)															// if the difference is one bit, then enter loop
					{
						number z;
						z.specifybit(numofvar);
						matrix[start][i].setused(true);							// mark the first number as used
						matrix[start][j].setused(true);							// mark the second number as used
						x = matrix[start][j].returnstring();
						for (int a = 0; a < numofvar; a++)
							if (matrix[start][i].returnstring()[a] != matrix[start][j].returnstring()[a])
							{
								x.replace(a, 1, "-");							// change the different bit with a dash
							}
						z.setbinary(x);
						z.push(matrix[start][j].returndec());						// push decimals that make up the new binary from the first binary
						z.push(matrix[start][i].returndec());						// push decimals that make up the new binary from the second binary
						matrix[start + 1].push_back(z);								// push the new binary with the dash into col after
						end = false;
					}
					count = 0;
				}

			}

		}

		start++;
	}
	eliminate(matrix);						// sometimes the col has repeated values with the same binary term and thus, this function eliminates these reptitions 
}
void eliminate(vector<vector<number>> & matrix)
{
	int start = 0;
	string x;
	vector<int> pos;		// array with the positions of the repeated terms
	while (start < 16 && matrix[start].size() != 0)
	{

		for (int i = 0; i < matrix[start].size() - 1; i++)
		{
			for (int j = i + 1; j < matrix[start].size(); j++)
			{
				if (matrix[start][i].returnstring() == matrix[start][j].returnstring())
				{
					pos.push_back(j);														// pushing locations of the repeated terms
				}

			}

		}
		for (int i = 0; i < pos.size(); i++)
			matrix[start].erase(matrix[start].begin() + (pos[i] - i));				// erasigng the positions of the repeated terms
		pos.clear();
		start++;
	}

}
void printimp(vector<vector<number>> &v, vector<number>& primeimp)
{
	int j = 0;
	vector<int> decimals;			// array with the decimal digits that make up the binary value
	cout << endl << "The Implication Table is: "<<endl;
	while (j < 16 && v[j].size() != 0)				// while we didn't reach the 16th col and that the colm is not empty
	{
		cout << endl << "Column " << j + 1 << endl;			// print col number

		for (int i = 0; i < v[j].size(); i++)
		{
			if (v[j][i].returnused())		// if the value is used, print an extra asterisk
			{
				decimals = v[j][i].returndec();
				for (int i = 0; i < decimals.size(); i++)
					cout << decimals[i] << ",";				// print the decimal values that make up this digit
				cout << "    " << v[j][i].returnstring() << " *" << endl;		// print the binary term
			}
			else {												// if the values wasn't used then it is a prime implicant and no asterisk is printed next to it
				primeimp.push_back(v[j][i]);
				decimals = v[j][i].returndec();
				for (int i = 0; i < decimals.size(); i++)
					cout << decimals[i] << ","; cout << "    " << v[j][i].returnstring()  << endl;	// print the decimal and next to it the binary value
			}
		}
		j++;
	}
	cout << endl << "The prime implicants are: " << endl << endl;
	for(int i=0;i<primeimp.size();i++)
		cout << primeimp[i].returnstring() << "       " << binarytoalpha(primeimp[i].returnstring()) << endl;

}

void printcov(vector<vector<number>> &covtable)
{
	cout << endl << "The Coverage Table is: " << endl << endl;

	for (int i = 0; i < covtable[0].size(); i++)
	{	
		for (int j = 0; j < covtable.size() && covtable[j].size()!=0; j++)
		{	
			cout << covtable[j][i].returnstring();
			cout << setw(10);
		}
		cout << endl;
	}
	cout << endl;
}
void minimiziedexp(vector<vector<number>> covtable, vector<number>& essprimeimp, vector<number>& min, vector<int>& rows)
{
	vector<int> cols;
	int c;
	for (int j = 0; j < rows.size(); j++)
	{
		c = 1;
		while (c <  pow(2, 16) && covtable[c].size() != 0)
		{
			if (covtable[c][rows[j]].returnstring() == "x")
			{
				covtable[c][rows[j]].setbinary("&");
				cols.push_back(c);
			}
			c++;
		}

	}



	for (int i = 0; i<cols.size(); i++)
	{
		for (int j = 1; j < covtable[cols[i]].size(); j++)
		{
			if (covtable[cols[i]][j].returnstring() == "x")
			{
				covtable[cols[i]][j].setbinary("&");
			}
		}

	}



	c = 1;
	int count = 0;
	int store;
	vector<number> temp;
	for (int i = 1; i<covtable[0].size(); i++)
	{
		c = 1;
		while (c <  pow(2, 16) && covtable[c].size() != 0)
		{
			if (covtable[c][i].returnstring() == "&")
				count++;

			c++;
		}
		if (count == 0)
		{
			temp.push_back(covtable[0][i]);
		}
		count = 0;
	}

	for (int i = 0; i < essprimeimp.size(); i++)
	{
		min.push_back(essprimeimp[i]);
	}
	for (int i = 0; i < temp.size(); i++)
	{
		min.push_back(temp[i]);
	}

	cout <<endl<< "The simplified expression is: "  << endl;
	for (int i = 0; i <min.size(); i++)
	{	if(i==0)
		cout << binarytoalpha(min[i].returnstring());
	else cout << " + " << binarytoalpha(min[i].returnstring());
	}
	cout << endl<<endl;
}
void printess(vector<vector<number>>& covtable, vector<number>& essprimeimp, vector<int>& rows)
{
	int c = 1;
	int count = 0;
	int temp;


	while (c < pow(2, 16) && covtable[c].size() != 0)
	{
		for (int j = 1; j < covtable[c].size(); j++)
		{
			if (covtable[c][j].returnstring() == "x")
			{
				count++;
				temp = j;

			}
		}

		if (count == 1)
		{
			essprimeimp.push_back(covtable[0][temp]);
			rows.push_back(temp);

		}
		count = 0;
		c++;
	}

	cout << "The essential prime implicants are: " << endl;
	for (int i = 0; i<essprimeimp.size(); i++)
		cout << essprimeimp[i].returnstring()<<"         " <<binarytoalpha(essprimeimp[i].returnstring())<< endl;



}
void createcovchart(vector<number>& minterms, vector<number>& primeimplicants, vector<vector<number>> &covtable)
{
	int col = 0;
	int c;
	number temp;
	bool flag = false;
	temp.setbinary("           ");
	covtable[0].push_back(temp);

	for (int i = 0; i<primeimplicants.size(); i++)
	{
		covtable[0].push_back(primeimplicants[i]);
	}

	while (col < minterms.size())
	{
		covtable[col + 1].push_back(minterms[col]);
		col++;
	}

	for (int i = 1; i<covtable[0].size(); i++)
	{
		for (int j = 0; j< covtable[0][i].returndec().size(); j++)
		{
			c = 1;
			while (c <  pow(2, 16) && covtable[c].size() != 0 && !flag)
			{
				if (covtable[0][i].returndec()[j] == covtable[c][0].returndec()[0])
				{
					temp.setbinary("x");
					covtable[c].push_back(temp);
					flag = true;
				}
				c++;
			}
			flag = false;
		}

		c = 1;
		while (c < pow(2, 16) && covtable[c].size() != 0)
		{

			if (covtable[c].size() < i + 1)
			{
				temp.setbinary("-");
				covtable[c].push_back(temp);
			}
			c++;
		}
	}
}
