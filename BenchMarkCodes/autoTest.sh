
CppTest()
{
    for i in 0 2 3
    do
        echo g++-11 -O$i
        g++-11 --std=c++11 -W -o 12b.out 12b.cpp -O$i
        for j in {1..3}
        do
            echo running $j of 3
            ./12b.out
        done
        echo ---------------------
    done
}

PyJuMatTest()
{
    echo Python
    for j in {1..3}
    do
        echo running $j of 3
        python3 12b.py
    done
    echo ---------------------
    echo Julia
    for j in {1..3}
    do
        echo running $j of 3
        julia 12b.jl
    done
    echo ---------------------
    echo Octave
    for j in {1..3}
    do
        echo running $j of 3
        octave 12b.m
    done
}


#must start with 30
echo 30
CppTest
PyJuMatTest
echo ==============================

# 30 to 300
echo 30 to 300
for FILE in 12b.py 12b.m 12b.cpp 12b.jl
do
    sed -i'.bak' -e 's/= 30/= 300/' $FILE
done

CppTest
PyJuMatTest
echo ==============================

# 300 to 3000
echo 300 to 3000
for FILE in 12b.py 12b.m 12b.cpp 12b.jl
do
    sed -i'.bak' -e 's/= 300/= 3000/' $FILE
done

CppTest
PyJuMatTest
echo ==============================

# 3000 to 30000
echo 3000 to 30000
for FILE in 12b.py 12b.m 12b.cpp 12b.jl
do
    sed -i'.bak' -e 's/= 3000/= 30000/' $FILE
done

CppTest
PyJuMatTest
echo ==============================

# 3000 to 30000
echo 30000 to 300000
for FILE in 12b.py 12b.m 12b.cpp 12b.jl
do
    sed -i'.bak' -e 's/= 30000/= 300000/' $FILE
done

CppTest
PyJuMatTest
echo ==============================