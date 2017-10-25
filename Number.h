#ifndef NUMBER_H
#define NUMBER_H

class number		// each term in the quine mcluskey is represented by a binary form, bool if it is used or not, the number of ones in it, and the number of bits
{
private:

	string binary;				// binary form
	int decimal, ones, bits;	// numer of ones and bits
	bool used;					// used flag
	vector<int> numbers;		// array of the decimals that the binary is composed of
	void convert()
	{
		binary = bitset<16>(decimal).to_string();
		
		
		if (bits < 16)
		{
			for (int i = 0; i < 16-bits; i++)
				binary.erase(0, 1);
		}
		for (int i = 0; i < binary.length(); i++)
		{
			if (binary[i] == '1')
				ones++;
		}
	}
	
public:

	number()
	{
		binary = "";
		decimal = 0;
		ones = 0;
		bits = 0;
		used = false;
	}
	void specifybit(int x)
	{	
		bits = x;						// specify the number of bits/ number of variables
	}			
	void insert(int x)
	{
		decimal = x;
		ones = 0;						// when given the decimal number, it calculates the numer of ones, its binary valye and initalizes used with false
		used = false;
		convert();
		numbers.push_back(x);
	}
	void setused(bool x)
	{
		used = x;						// set used flag with either true or false
	}
	void setbinary(string x)
	{
		decimal = 0;
		binary = x;
		ones = 0;
		for (int i = 0; i < x.length(); i++)					// used for the dashes binary form. it counts the number of ones and initialized the used with false
		{
			if (x[i] == '1')
				ones++;
		}
		used = false;
	}
	string returnstring()
	{										// return binary form
		return binary;
	}
	int returnones()
	{
		return ones;					// return the number of ones
	}
	bool returnused()
	{
		return used;						// return the used flag
	}
	vector<int> returndec()
	{	
		return numbers;					// returns all the values that the binary is composed of
	}
	void push(vector<int> n)
	{
		for (int i = 0; i < n.size(); i++)
			numbers.push_back(n[i]);			// pushes the decimals that the binary is made of to the array of the number

	}

};

#endif