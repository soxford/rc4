g++ -std=c++11 arrayify.cpp -o arrayify
openssl rc4 -e -K "000102030405060708090A0B0C0D0E0F" -in 16encryptionInput.bin -out encryptionOutput.bin
./arrayify
openssl rc4 -e -K "01234567891011121314151617181920" -in 16encryptionInput.bin -out encryptionOutput.bin
./arrayify
openssl rc4 -e -K "0F0E0D0C0B0A09080706050403020100" -in 16encryptionInput.bin -out encryptionOutput.bin
./arrayify
openssl rc4 -e -K "000102030405060708090A0B0C0D0E0F" -in 257encryptionInput.bin -out encryptionOutput.bin
./arrayify
openssl rc4 -e -K "01234567891011121314151617181920" -in 257encryptionInput.bin -out encryptionOutput.bin
./arrayify
openssl rc4 -e -K "0F0E0D0C0B0A09080706050403020100" -in 257encryptionInput.bin -out encryptionOutput.bin
./arrayify
rm encryptionOutput.bin
rm arrayify
