add_library(re2 STATIC
  ${OSSIA_3RDPARTY_FOLDER}/re2/re2/bitstate.cc
  ${OSSIA_3RDPARTY_FOLDER}/re2/re2/compile.cc
  ${OSSIA_3RDPARTY_FOLDER}/re2/re2/dfa.cc
  ${OSSIA_3RDPARTY_FOLDER}/re2/re2/filtered_re2.cc
  ${OSSIA_3RDPARTY_FOLDER}/re2/re2/mimics_pcre.cc
  ${OSSIA_3RDPARTY_FOLDER}/re2/re2/nfa.cc
  ${OSSIA_3RDPARTY_FOLDER}/re2/re2/onepass.cc
  ${OSSIA_3RDPARTY_FOLDER}/re2/re2/parse.cc
  ${OSSIA_3RDPARTY_FOLDER}/re2/re2/perl_groups.cc
  ${OSSIA_3RDPARTY_FOLDER}/re2/re2/prefilter.cc
  ${OSSIA_3RDPARTY_FOLDER}/re2/re2/prefilter_tree.cc
  ${OSSIA_3RDPARTY_FOLDER}/re2/re2/prog.cc
  ${OSSIA_3RDPARTY_FOLDER}/re2/re2/re2.cc
  ${OSSIA_3RDPARTY_FOLDER}/re2/re2/regexp.cc
  ${OSSIA_3RDPARTY_FOLDER}/re2/re2/set.cc
  ${OSSIA_3RDPARTY_FOLDER}/re2/re2/simplify.cc
  ${OSSIA_3RDPARTY_FOLDER}/re2/re2/stringpiece.cc
  ${OSSIA_3RDPARTY_FOLDER}/re2/re2/tostring.cc
  ${OSSIA_3RDPARTY_FOLDER}/re2/re2/unicode_casefold.cc
  ${OSSIA_3RDPARTY_FOLDER}/re2/re2/unicode_groups.cc
  ${OSSIA_3RDPARTY_FOLDER}/re2/util/rune.cc
  ${OSSIA_3RDPARTY_FOLDER}/re2/util/strutil.cc
)

set_target_properties(re2 PROPERTIES UNITY_BUILD 0)
if(MSVC)
  target_compile_definitions(re2 PRIVATE /utf-8)
endif()

if(WIN32)
  target_compile_definitions(re2 PRIVATE -DUNICODE -D_UNICODE -DSTRICT -DNOMINMAX)
  target_compile_definitions(re2 PRIVATE -D_CRT_SECURE_NO_WARNINGS -D_SCL_SECURE_NO_WARNINGS)
endif()

target_include_directories(re2 PUBLIC $<BUILD_INTERFACE:${OSSIA_3RDPARTY_FOLDER}/re2>)

if(UNIX)
  target_link_libraries(re2 PUBLIC Threads::Threads)
endif()

if(OSSIA_INSTALL_STATIC_DEPENDENCIES)
  install(
    TARGETS re2
    EXPORT re2-exports
    RUNTIME DESTINATION lib
    ARCHIVE DESTINATION lib)

  install(EXPORT re2-exports
          DESTINATION lib/cmake/re2)
  export(EXPORT re2-exports)
endif()
