// The encryption program in C++ and ASM with a very simple encryption method - it simply adds 1 to the character.
// The encryption method is written in ASM. You will replace this with your allocated version for the assignment.
// In this version parameters are passed via registers (see 'encrypt' for details).
// Filename: "4473 FoCA 2015 Encryption Original with ASM.cpp"
//
// Teto Hussein, last update: Mar 2015
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

void get_char (char& a_character)
{
	cin >> a_character;
	while (((a_character < '0') | (a_character > 'z')) && (a_character != dollarchar))
	{	cout << "Alphanumeric characters only, please try again > ";
		cin >> a_character;
	}
}
//-------------------------------------------------------------------------------------------------------------

void get_original_chars (int& length)
{	char next_char;
	length = 0;
	get_char (next_char);

	while ((length < MAXCHARS) && (next_char != dollarchar))
	{
		OChars [length++] = next_char;
		get_char (next_char);
	}
}

//---------------------------------------------------------------------------------------------------------------
//----------------- ENCRYPTION ROUTINES -------------------------------------------------------------------------

void encrypt_chars (int length, char EKey)
{   char temp_char;						// char temporary store

	for (int i = 0; i < length; i++)	// encrypt characters one at a time
	{
		temp_char = OChars [i];			//
		__asm {							//
			push   eax					// save register values on stack to be safe
			push   ecx					//
										//
			movzx  ecx,temp_char		// set up registers (Nb this isn't StdCall or Cdecl)	
			push ecx					// second parameter in the stack(reverse order rule)
			lea    eax,EKey				//			
			push eax					// first paremeter in the stack (reverse order rule)
			call   encrypt13			// encrypt the character
			add esp,8					// This will clear the parameters pushed onto the stack after the subroutine
			mov    temp_char,al			//
										// HI
			pop    ecx					// restore original register values from stack
			pop    eax					//
		}
		EChars [i] = temp_char;			// Store encrypted char in the encrypted chars array
	}
   return;


	// Encrypt subroutine. You should paste in the encryption routine you've been allocated from Bb and
	// overwrite this initial, simple, version. Ensure you change the ‘call’ above to use the
	// correct 'encryptnn' label where nn is your encryption routine number.
	// Inputs: register EAX = 32-bit address of Ekey,
	//					ECX = the character to be encrypted (in the low 8-bit field, CL).
	// Output: register EAX = the encrypted value of the source character (in the low 8-bit field, AL).
   __asm {

   encrypt13:
	   push ebp					// Save the value of the base pointer into the stack
	   mov ebp,esp				// This set the base pointer to the point where we pushed our parameters into the stack
	   mov ecx, [ebp+12]		// ecx will obtain the EKey from the second parameter
	   mov eax, [ebp+8]			// eax will obtain the temp_char fron the first parameter
	   push ebx					// Save register ebx value on stack to be safe
	   push  ecx				// Save the ecx register on stack content which is currently holding the temp_char value that I need to encrypt
	   mov  ebx, [eax]			// Saves the memory address content(i.e. Ekey) from the register eax into ebx
	   xchg eax, ebx			// This will swap the value between ebx and eax so that eax will now hold the Ekey value
	   and eax, 0xFF			// Anding 255 to the value in eax register which should have the Ekey value in it
	   ror al, 1				// This will rotate the low 8 bits field of the EAX register by 1 place  e.g.10000001 will turn to 11000000
	   add eax, 0x01			// Add one to the value that has been rotated in eax to enhance the encryption
	   mov edx, eax				// copying eax value into edx register
	   xchg eax, ebx			// swap the value between ebx and eax so now eax will get it is previous value and ebx will hold the encrypted characer
	   mov[eax], edx			// move the content of edx which hold the value encrypted so far into the memory address of eax
	   and edx, 0x0F			// and 15 in bitwise with edx which hold the value encrypted so far to further enhance the encryption
	   pop eax					// return the previous value of eax that was on the stack which is the temp_char value
	   add eax, edx				// Add edx into eax
	   xor eax, edx				// exclusive or edx in eax to get the tempo_char value
	   pop ebx					// Get the original value of ebx from the stack
	   mov esp, ebp				// set the stack pointer position to where the base pointer where when we pushed it
	   pop ebp					// restore the original value of ebp
	   ret						// return to the position after the call
	}

	//--- End of Assembly code
}
// end of encrypt_chars function
//---------------------------------------------------------------------------------------------------------------




//---------------------------------------------------------------------------------------------------------------
//----------------- DECRYPTION ROUTINES -------------------------------------------------------------------------
//
void decrypt_chars (int length, char EKey)
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

		cout << "\nPlease enter your Encryption Key (EKey) letter: "; get_char (EKey);

		cout << "\nNow enter upto " << MAXCHARS << " alphanumeric characters:\n";
		get_original_chars (char_count);
		cout << "\n\nOriginal source string = " << OChars << "\tHex = ";
		for (int i=0; i<char_count; i++) cout<<hex<<setw(2)<<setfill('0')<< ((int (OChars[i]))&0xFF)<<"  ";

		encrypt_chars (char_count, EKey);
		cout << "\n\nEncrypted string       = " << EChars << "\tHex = ";
		for (int i=0; i<char_count; i++) cout<< ((int (EChars[i]))&0xFF)<<"  ";

		decrypt_chars (char_count, EKey);
		cout << "\n\nDecrypted string       = " << DChars << "\tHex = ";
		for (int i=0; i<char_count; i++) cout<< ((int (DChars[i]))&0xFF)<<"  ";

	cout << "\n\nPress a key to end...";
	while ( !_kbhit());					//hold the screen until a key is pressed
	return (0);


} // end of whole encryption/decryption program --------------------------------------------------------------------


