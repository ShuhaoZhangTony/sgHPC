 #!/bin/bash
rm pro_1_4
   echo "compile File"
   g++ -o  pro_1_4 pro_1_4.cpp -lm -fopenmp
   ./pro_1_4 13

