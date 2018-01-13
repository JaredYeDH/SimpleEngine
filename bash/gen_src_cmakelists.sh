cd '/Users/oceancx/MHXY/SimpleEngine/bash'
find ../src  \( -name  "*.h" -o -name "*.cpp"  -o -name "*.inl" \)  -print > tmp2.txt
sed  's#../src/##g' tmp2.txt >tmp.txt
sed  '/add_executable/ r tmp.txt' SRC_CMakeLists_Template.txt > ../src/CMakeLists.txt
rm tmp.txt tmp2.txt
