FILE(REMOVE_RECURSE
  "doc/html"
  "CMakeFiles/doc"
  "doc/html/index.html"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang)
  INCLUDE(CMakeFiles/doc.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)
