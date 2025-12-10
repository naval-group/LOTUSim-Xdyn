echo "#ifndef stb_guard" >stb.hpp
echo "#define stb_guard" >>stb.hpp
echo "" >>stb.hpp
echo "#define STB_IMAGE_IMPLEMENTATION" >>stb.hpp
echo "" >>stb.hpp
echo "#pragma GCC system_header" >>stb.hpp
echo "" >>stb.hpp
for header in $(ls stb/*.h*); do
  echo "#include \"${header}\"" >>stb.hpp
done
echo "" >>stb.hpp
echo "#endif" >>stb.hpp
