cd 'D:/Github/SimpleEngine/bash'
find ../src  \( -name  "*.h" -o -name "*.cpp"  -o -name "*.inl" \)  -print>tmp.txt
sed  -i 's#../src/##g' tmp.txt
sed  '/add_executable/ r tmp.txt' SRC_CMakeLists_Template.txt > ../src/CMakeLists.txt
rm tmp.txt
