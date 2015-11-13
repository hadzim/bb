cmake_minimum_required(VERSION 3.0)
project(BB.WWW)

file(GLOB_RECURSE ${PROJECT_NAME}.Sources "src/*.cpp")
add_executable(${PROJECT_NAME} ${${PROJECT_NAME}.Sources})

target_include_directories(${PROJECT_NAME}
	PUBLIC include
	PRIVATE src
	PRIVATE ${PROJECT_BINARY_DIR}
)

target_link_libraries(${PROJECT_NAME}
	TBS
	TBS.Services
	BB
)

installDefault(${PROJECT_NAME} .)
servicefile()

# -----------------------
# CAT FUNCTION

function(cat IN_FILE OUT_FILE)
  file(READ ${IN_FILE} CONTENTS)
  file(APPEND ${OUT_FILE} "${CONTENTS}")
endfunction()

# -----------------------
# JS

# Prepare a temporary file to "cat" to:
file(WRITE all.js.in "")

#jquery
cat(scripts/js/jquery/jquery.js all.js.in)

#other libs
file(GLOB_RECURSE RC.JS.Sourcesa "scripts/js/libs/1/*.js")
foreach(PACKAGE_SQL_FILE ${RC.JS.Sourcesa})
  cat(${PACKAGE_SQL_FILE} all.js.in)
endforeach()

file(GLOB_RECURSE RC.JS.Sourcesa "scripts/js/libs/2/*.js")
foreach(PACKAGE_SQL_FILE ${RC.JS.Sourcesa})
  cat(${PACKAGE_SQL_FILE} all.js.in)
endforeach()


cat(scripts/js/app.js all.js.in)

#models
file(GLOB_RECURSE RC.JS.Sourcesa "scripts/js/models/*.js")
foreach(PACKAGE_SQL_FILE ${RC.JS.Sourcesa})
  cat(${PACKAGE_SQL_FILE} all.js.in)
endforeach()

#views
file(GLOB_RECURSE RC.JS.Sourcesa "scripts/js/views/*.js")
foreach(PACKAGE_SQL_FILE ${RC.JS.Sourcesa})
  cat(${PACKAGE_SQL_FILE} all.js.in)
endforeach()

# Copy the temporary file to the final location
configure_file(all.js.in all.js COPYONLY)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/all.js DESTINATION Default/data/minimal/js COMPONENT Default)


# -----------------------
# CSS

# Prepare a temporary file to "cat" to:
file(WRITE all.css.in "")

file(GLOB_RECURSE RC.CSS.Sources "scripts/css/libs/*.css")
foreach(PACKAGE_SQL_FILE ${RC.CSS.Sources})
  cat(${PACKAGE_SQL_FILE} all.css.in)
endforeach()

file(GLOB_RECURSE RC.CSS.Sources "scripts/css/*.css")
foreach(PACKAGE_SQL_FILE ${RC.CSS.Sources})
  cat(${PACKAGE_SQL_FILE} all.css.in)
endforeach()

# Copy the temporary file to the final location
configure_file(all.css.in all.css COPYONLY)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/all.css DESTINATION Default/data/minimal/css COMPONENT Default)

# -----------------------
# TEMPLATES

# Prepare a temporary file to "cat" to:
file(WRITE all.html.in "")

cat(scripts/indexBegin.html all.html.in)

file(GLOB_RECURSE RC.TPL.Sources "scripts/tpl/*.html")
foreach(PACKAGE_SQL_FILE ${RC.TPL.Sources})
  cat(${PACKAGE_SQL_FILE} all.html.in)
endforeach()

cat(scripts/indexEnd.html all.html.in)


# Copy the temporary file to the final location
configure_file(all.html.in index.html COPYONLY)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/index.html DESTINATION Default/data/minimal/ COMPONENT Default)


#HTML
install(DIRECTORY data DESTINATION Default COMPONENT Default)