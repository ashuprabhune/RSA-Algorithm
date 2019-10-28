#include <iostream>

#include <stdio.h>

#include <math.h>

#include <random>

#include <bits/stdc++.h>

using namespace std;

signed long long int s_Einv;

unsigned long long int modPower(unsigned long long int x, unsigned long long int p, unsigned long long int n) {
    unsigned long long int res = 1;
    x = x % n;
    while (p > 0) {
        if (p % 2 != 0) {
            res = (res * x) % n;
            p = p - 1;
        }
        p = p >> 1;
        x = (x * x) % n;

    }
    return res;
}

unsigned long long int power(unsigned long long int x, unsigned long long int p) {
    unsigned long long int res = 1;
    while (p > 0) {
        if (p % 2 != 0) {
            res = (res * x);
            p = p - 1;
        }
        p = p >> 1;
        x = (x * x);

    }
    return res;
}

bool millerRabinTest(int d, int n) {
    int a = 2 + rand() % (n - 4);
    int x = modPower(a, d, n);

    if (x == 1 || x == n - 1)
        return true;

    while (d != n - 1) {
        x = (x * x) % n;
        d *= 2;

        if (x == 1)
            return false;
        if (x == n - 1)
            return true;
    }
    return false;
}

bool isPrime(unsigned long long int n, int accuracyLevel) {
    if (n <= 1 || n == 4)
        return false;
    if (n <= 3)
        return true;
    int d = n - 1;
    while (d % 2 == 0)
        d /= 2;
    for (int i = 0; i < accuracyLevel; i++) {
        if (!millerRabinTest(d, n))
            return false;
    }
    return true;
}

unsigned long long int generatePrimeNos() {
    bool flag = 1;
    unsigned long long int number;
    while (flag) {
        number = rand() % 65536 + 2;
        if (isPrime(number, 5)) {
            flag = 0;
        }
    }
    return number;
}

unsigned long long int gcdExtended(unsigned long long int a, unsigned long long int b, signed long long int * s, signed long long int * t) {
    unsigned long long int r, q;
    signed long long int stemp;
    if (b == 0) {
        * s = 1;
        * t = 0;
        return a;
    }

    r = a % b;
    q = a / b;

    unsigned long long int g = gcdExtended(b, r, s, t);
    stemp = * s;
    * s = * t;
    * t = stemp - ( * t) * q;

    if (g == 1) {
        s_Einv = * s;
    }

    return g;

}

unsigned long long int bearcatiiEncoding(string message) {
    unsigned long long int total = 0;
    int t;
    for (int i = 0; i < (int) message.length(); i++) {
        if (message[i] != ' ') {
            if (message[i] >= 'A' && message[i] <= 'Z') {
                t = message[i] - 'A' + 1;
            } else if (message[i] >= 'a' && message[i] <= 'z') {
                t = message[i] - 'a' + 1;
            }
            total += t * power(27, (int) message.length() - i - 1);
        }
    }
    return total;
}

string bearcatiiDecoding(long encodedMessage) {
    string decodedString;
    while (encodedMessage != 0) {
        unsigned long long int r = encodedMessage % 27;
        encodedMessage /= 27;
        char x1 = 'A' + r - 1;
        if (x1 == '@')
            decodedString = ' ' + decodedString;
        else
            decodedString = x1 + decodedString;
    }
    return decodedString;
}

int main() {
    srand(time(0));
    unsigned long long int p, q, n, gcd, publicKey, privateKey, phi, encodedPlainText;
    string plainText, decodedCiperText;

    p = generatePrimeNos();
    q = generatePrimeNos();
    n = (p * q);

    while (n < 14348906 && n > 4294967297) {
        p = generatePrimeNos();
        q = generatePrimeNos();
        n = (p * q);
    }

    phi = (p - 1) * (q - 1);

    std::cout << "Prime Number(p): " << p << std::endl;
    std::cout << "Prime Number(q): " << q << std::endl;
    std::cout << "n(p * q): " << n << std::endl;
    std::cout << "phi(n): " << phi << std::endl;

    signed long long int x, y;
    std::cout << "Enter Public key: ";
    while (true) {
        cin >> publicKey;
	gcd = gcdExtended(publicKey, phi, & x, & y);
        if (!cin) 
	{
            cin.clear();
            cin.ignore(numeric_limits < streamsize > ::max(), '\n');
            std::cout << "Invalid integer!!! Enter Public Key Again: ";
            continue;
        }
	else if (gcd != 1)
	{
	    cin.clear();
            cin.ignore(numeric_limits < streamsize > ::max(), '\n');
            std::cout << "Invalid Public Key!!! Enter Public Key Again: ";
            continue;
	}
	else 
	    break;
    }

    if (s_Einv < 0) {
        privateKey = (unsigned long long int)(phi + s_Einv);
    } else {
        privateKey = (unsigned long long int)(s_Einv);
    }

    std::cout << "Enter your message(M): ";
    std::cin >> plainText;

    encodedPlainText = bearcatiiEncoding(plainText);

    unsigned long long int encryptedMessage = modPower(encodedPlainText, publicKey, n);
    std::cout << "Encrypyted Message(C): " << encryptedMessage << std::endl;

    unsigned long long int decryptedMessage = modPower(encryptedMessage, privateKey, n);

    decodedCiperText = bearcatiiDecoding(decryptedMessage);
    std::cout << "Decrypted message(P): " << decodedCiperText << std::endl;

    return 0;
}
