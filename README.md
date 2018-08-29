# permutemd5

A small utility to automate some username guessing for a webapp
pen testing challenge.

Given an md5 hash of a password and a seed hint, try permuting
the seed using "clever password tricks" and see if the resulting
hash matches the target.

The target hash and output hashes are hex only, not base64 encoded.

Requires libmhash.

Build with make

    ~/permutemd5$ make
    gcc -O2 -Wall permutemd5.c -lmhash -o permutemd5


    ~/permutemd5$ ./permutemd5
    A seed string is required.

    Usage: ./permutemd5 [-t <target>] [-v] seed
      -t       Target hash. Program stops if hash of the permuted value matches.
      -v       Verbose output. Always true if no target provided.
      seed     The seed string to be modified and hashed.


Some examples

The target

     ~/permutemd5$ echo -n "pa55w0rD!" | md5sum
     c0bba2a3b4b6b476317f5d5255762ae9  -

Can we find it?

     ~/permutemd5$ ./permutemd5 -t c0bba2a3b4b6b476317f5d5255762ae9 password
     Success!
     pa55w0rD! : c0bba2a3b4b6b476317f5d5255762ae9

Another target

     ~/permutemd5$ echo -n "user123" | md5sum
     6ad14ba9986e3615423dfca256d04e3f  -

     ~/permutemd5$ ./permutemd5 -t 6ad14ba9986e3615423dfca256d04e3f user
     Success!
     user123 : 6ad14ba9986e3615423dfca256d04e3f


If you don't provide a target, the permutations and hashes will be output.

    ~/permutemd5$ ./permutemd5 admin
    admin : 21232f297a57a5a743894a0e4a801fc3
    admin1 : e00cf25ad42683b3df678c61f42c6bda
    admin2 : c84258e9c39059a89ab77d846ddab909
    admin3 : 32cacb2f994f6b42183a1300d9a3e8d6
    admin4 : fc1ebc848e31e0a68e868432225e3c82
    admin5 : 26a91342190d515231d7238b0c5438e1
    admin6 : c6b853d6a7cc7ec49172937f68f446c8
    admin7 : 788073cefde4b240873e1f52f5371d7d
    admin8 : 8762eb814817cc8dcbb3fb5c5fcd52e0
    admin9 : eed57216df3731106517ccaf5da2122d
    admin123 : 0192023a7bbd73250516f069df18b500
    admin! : 412f58bde15d78d2c6e81d2bc4d5ba40
    admin? : b57f08daa5ddc7b443d15e0fb9b80d26
    admin$ : f90d1250fd96b918b6d474a2e549510c
    admin# : 32a092884ced5c25690ff1a1165da3f2
    admiN : 806d8e1e323557a073a79d006c3fae11
    admiN1 : 42377266a9310df0f6870ec64f10a743
    ...
    aDm1N$ : 398d9d293b6e0f63d0be6642693f7697
    aDm1N# : de7bd3decbd59012fe46e40351b6eca4
    aDm!n : 1bb5457c4d003ff38a50c4dcfbec6293
    aDm!n1 : 6a92c5d6da21638aa1d805ca4d9371dc
    aDm!n2 : 80864ed6c87a1e33b956626dae5ce242
    aDm!n3 : 4fd232a85de8b0adddb29bf9fc72d14b
    aDm!n4 : d35faef1ebf8ac4e179a7d2e308b65c6
    aDm!n5 : 8387091a3cf1a104c39a05b63348f044
    aDm!n6 : c4210a236bc919e024c4affef7cdf314
    aDm!n7 : f257cb30744f0e97388c42bfdba3723b
    ...
    ADM!N123 : 37f2a426855c79818cd52d787dce9b58
    ADM!N! : 1acc1cd3c2e05ad09bcfe96190dc46c1
    ADM!N? : 44649b491573b1ed421738d33b9c2616
    ADM!N$ : 44331bc279860c5a97064829a5400ce4
    ADM!N# : 31708a540ee877b105ef8267bb6446fe

