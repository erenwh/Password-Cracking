#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>

#include "key.hpp"
#include "brute.hpp"
#include "timer.hpp"

std::string me;
std::string encrypted;
std::string table_filename;
bool verbose = false;
std::vector<std::string> decrypted;

Brute::Brute(const std::string& filename) {
	T.resize(N);
	std::string buffer;
    std::fstream input(filename.c_str(), std::ios::in);
    for (int i = 0; i < N; i++) {
        std::getline(input, buffer);
        T[i].set_string(buffer);
    }
    input.close();
}

void Brute::decrypt(const std::string& encrypted){
    // Use a brute force algorithm to decrypt the encrypted key_to_decrypt.

	// Because the encrypted password is only 5-char in length, we could brute-force and try to try every possibility
	// from the table 32*32*32*32*32 = 33 mill

	/*Key targetKey((encrypted));		// encrypted key
	Key ssum;						// decrypted key
	for (int i = 0; i < 32; ++i) {
		for (int j = 0; j < 32; ++j) {
			for (int k = 0; k < 32; ++k) {
				for (int l = 0; l < 32; ++l) {
					for (int m = 0; m < 32; ++m) {
						std::string temp;
						temp.push_back(ALPHABET[i]);
						temp.push_back(ALPHABET[j]);
						temp.push_back(ALPHABET[k]);
						temp.push_back(ALPHABET[l]);
						temp.push_back(ALPHABET[m]);	// construct key
						//std::cout << temp << std::endl;
						Key passkey((temp));
						ssum = passkey.subset_sum(T, verbose);
						if (ssum == targetKey) {
							decrypted.push_back(temp);
						}
					}
				}
			}
		}
	}*/
    // modified

    CPU_timer t;
    t.tic();
    // try all possible subsets of T
    Key candidate;         // a password candidate
    Key encry = Key(encrypted);                 // the encrypted password
    Key candenc;                   // the encrypted password candidate
    Key zero;              // the all zero key
    do
    {
        candenc = candidate.subset_sum(T,verbose);
        if (candenc == encrypted)
            std::cout << candidate << std::endl;
        ++candidate;
    } while (candidate != zero);

    t.toc();
    double elapsed_secs = t.elapsed();

    std::cout << "Decryption took "
              << elapsed_secs
              << " Mill.seconds." << std::endl;

}

void usage(const std::string& error_msg="") {
	if (!error_msg.empty()) std::cout << "ERROR: " << error_msg << '\n';
	std::cout << me << ": Brute force cracking of Subset-sum password with " 
		<< B << " bits precision\n"
	    << "USAGE: " << me << " <encrypted> <table file> [options]\n"
		<< "\nArguments:\n"
		<< " <encrypted>:   encrypted password to crack\n"
		<< " <table file>:  name of file containing the table to use\n"
		<< "\nOptions:\n"
		<< " -h|--help:     print this message\n"
		<< " -v|--verbose:  select verbose mode\n\n";
	exit(0);
}

void initialize(int argc, char* argv[]) { // Initialize the class with the table contained in table_filename.
	me = argv[0];
	if (argc < 3) usage("Missing arguments");
	encrypted = argv[1];
	table_filename = argv[2];
	for (int i=3; i<argc; ++i) {
		std::string arg = argv[i];
		if (arg == "-h" || arg == "--help") usage();
		else if (arg == "-v" || arg == "--verbose") verbose = true;
		else usage("Unrecognized argument: " + arg);
	}

}


int main(int argc, char *argv[]){
    //Read in an encrypted password and the name of a file containing the table to b used
    //and decrypt it using the brute force algorithm

	initialize(argc, argv);

	Brute b(table_filename);

	// try to decrypt
	b.decrypt(encrypted);

	// print result
	for (int i = 0; i < decrypted.size(); ++i) {
		std::cout << decrypted[i] << '\n';
	}


	return 0;
}
