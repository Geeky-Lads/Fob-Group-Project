// The encryption program in C++ and ASM with a very simple encryption method - it simply adds 1 to the 

character.
// The encryption method is written in ASM. You will replace this with your allocated version for the assignment.
// In this version parameters are passed via registers (see 'encrypt' for details).
// Filename: "4473 FoCA 2015 Encryption Original with ASM.cpp"
//
// James Harris, last update: March 2015
//  *** PLEASE CHANGE THE LINE ABOVE TO YOUR NAME & DATE ***

#include <conio.h>		// for kbhit
#include <iostream>		// for cin >> and cout <<
#include <iomanip>		// for fancy output
using namespace std;

#define MAXCHARS 6		// feel free to alter this, but 6 is the minimum
#define dollarchar '$'  // string terminator

char OChars[MAXCHARS],
EChars[MAXCHARS],
DChars[MAXCHARS] = "Soon!";	// Global Original, Encrypted, Decrypted character strings

//----------------------------- C++ Functions ----------------------------------------------------------

void get_char(char& a_character)
{
	cin >> a_character;
	while (((a_character < '0') | (a_character > 'z')) && (a_character != dollarchar))
	{
		cout << "Alphanumeric characters only, please try again > ";
		cin >> a_character;
	}
}
//-------------------------------------------------------------------------------------------------------------

void get_original_chars(int& length)
{
	char next_char;
	length = 0;
	get_char(next_char);

	while ((length < MAXCHARS) && (next_char != dollarchar))
	{
		OChars[length++] = next_char;
		get_char(next_char);
	}
}

//---------------------------------------------------------------------------------------------------------------
//----------------- ENCRYPTION ROUTINES -------------------------------------------------------------------------

void encrypt_chars(int length, char EKey)
{
	char temp_char;						// declaring the temp_char as 

	a character variable

	for (int i = 0; i < length; i++)	// encrypt characters one at a time by working out the length of the 

		string entered
	{
		temp_char = OChars[i];			//Setting the temp_char variable to one 

		character at a time from the string entered until it's completed the length
			__asm {
			push   eax					// Save register values on 

				stack to be safe
				push   ecx				// Pushing the temp_char 

				onto the stack
				movzx ecx, temp_char		// loading "a" in ascii which is 

				the EKey into the eax register moving register address of Ekey into eax
				push   temp_char
				lea    eax, EKey
				push ecx
				push eax
				call   encrypt9				// calling the 

				encrypt9 subroutine to encrypt the string entered
				add	   esp, 8
				mov    temp_char, al

				pop    temp_char
				pop    EKey
				pop    ecx					// Restoring 
				pop    exc					// Coming

				original value of register ECX and removing from the stack
				pop    eax					// Restoring 

				orginial value of register EAX and removing from the stack
		}
		EChars[i] = temp_char;			// Store the encrypted character in the 

		encrypted chars array
	}
	return;


	// Encrypt subroutine. You should paste in the encryption routine you've been allocated from Bb and
	// overwrite this initial, simple, version. Ensure you change the ‘call’ above to use the
	// correct 'encryptnn' label where nn is your encryption routine number.
	// Inputs: register EAX = 32-bit address of Ekey,
	//					ECX = the character to be encrypted (in the low 8

	-bit field, CL).
		// Output: register EAX = the encrypted value of the source character (in the low 8-bit field, AL).
		__asm {

	encrypt9:

		push ebp
			mov ebp, esp
			mov ecx, [ebp + 12]			//Change the base pointer of the stack 

			to find the temp_char
			mov eax, [ebp + 8]		//Change the base pointer of the stack to find the 

			Ekey

			push ebx		//Pushing the Ekey onto the stack
			push ecx		//Pushing the temp_char value onto the stack, first character 

			of the string

			mov ebx, [eax]	//Anding the Ekey with  
			and ebx, 0x000000FF	//Anding the Ekey with 255 in decimal, it's padded 

			out with 0's because it's an 8 - bit reigster
			mov edx, 05			//Moving the value of 5 into the EDX 

			register which is initalised as the loop counter
		x9 : rol bl, 1		//loop excecuting 5 times
			 dec edx		//decrement loop counter
			 jnz x9			//Jump if not zero 9 times for the x loop
			 or  ebx, 04h
			 mov edx, ebx	//bitshifts the EKey
			 mov[eax], ebx
			 pop eax		//
		 y9 : rol al, 1		//loop excecuting 8 times starting from 1
			  dec edx		//decrement loop counter
			  jnz y9		//Jump if not zero 9 times for the y loop
			  pop ebx		//reset ebx to original value
			  mov esp, ebp
			  pop ebp
			  ret
	}

	//--- End of Assembly code
}
// end of encrypt_chars function
//---------------------------------------------------------------------------------------------------------------




//---------------------------------------------------------------------------------------------------------------
//----------------- DECRYPTION ROUTINES -------------------------------------------------------------------------
//
void decrypt_chars(int length, char EKey)
{
	/*** to be written by you ***/

	return;
}
// end of decrypt_chars function
//---------------------------------------------------------------------------------------------------------------





int main(void)
{
	int  char_count;	// The number of actual characters entered (upto MAXCHARS limit).
	char EKey;			// Encryption key.

	cout << "\nPlease enter your Encryption Key (EKey) letter: "; get_char(EKey);

	cout << "\nNow enter upto " << MAXCHARS << " alphanumeric characters:\n";
	get_original_chars(char_count);
	cout << "\n\nOriginal source string = " << OChars << "\tHex = ";
	for (int i = 0; i<char_count; i++) cout << hex << setw(2) << setfill('0') << ((int(OChars[i])) & 0xFF) << "  ";

	encrypt_chars(char_count, EKey);
	cout << "\n\nEncrypted string       = " << EChars << "\tHex = ";
	for (int i = 0; i<char_count; i++) cout << ((int(EChars[i])) & 0xFF) << "  ";

	decrypt_chars(char_count, EKey);
	cout << "\n\nDecrypted string       = " << DChars << "\tHex = ";
	for (int i = 0; i<char_count; i++) cout << ((int(DChars[i])) & 0xFF) << "  ";

	cout << "\n\nPress a key to end...";
	while (!_kbhit());					//hold the screen until a key is pressed
	return (0);


} // end of whole encryption/decryption program --------------------------------------------------------------------
