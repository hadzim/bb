cmake_minimum_required(VERSION 3.0)
project(www)

#####
##### CREATE CSS AND JS
#####

# -----------------------
# CAT FUNCTION

#function(cat IN_FILE OUT_FILE)
#  file(READ ${IN_FILE} CONTENTS)
#  file(APPEND ${OUT_FILE} "${CONTENTS}")
#endfunction()

# -----------------------
# CSS

# Prepare a temporary file to "cat" to:
#file(WRITE all.css.in "")

#file(GLOB_RECURSE RC.CSS.Sources "scripts/css/*.css")

# Call the "cat" function for each input file
#foreach(PACKAGE_SQL_FILE ${RC.CSS.Sources})
#  cat(${PACKAGE_SQL_FILE} all.css.in)
#endforeach()

# Copy the temporary file to the final location
#configure_file(all.css.in all.css COPYONLY)

#install(FILES ${CMAKE_CURRENT_BINARY_DIR}/all.css DESTINATION Default/data/remoteControl/css COMPONENT Default)

# -----------------------
# JS

# Prepare a temporary file to "cat" to:
#file(WRITE all.js.in "")


#cat(scripts/js/jquery.js all.js.in)


#file(GLOB_RECURSE RC.JS.Sourcesa "scripts/js/jquery.*.js")
#foreach(PACKAGE_SQL_FILE ${RC.JS.Sourcesa})
#  cat(${PACKAGE_SQL_FILE} all.js.in)
#endforeach()

#cat(scripts/js/bootstrap.js all.js.in)

#file(GLOB_RECURSE RC.JS.Sources "scripts/js/local*.js")
#foreach(PACKAGE_SQL_FILE ${RC.JS.Sources})
#  cat(${PACKAGE_SQL_FILE} all.js.in)
#endforeach()

# Copy the temporary file to the final location
#configure_file(all.js.in all.js COPYONLY)

#install(FILES ${CMAKE_CURRENT_BINARY_DIR}/all.js DESTINATION Default/data/remoteControl/js COMPONENT Default)

# -----------------------
# OTHER FILES

install(
	DIRECTORY public 
	DESTINATION Default/www
	COMPONENT Default)

install(
	FILES bb.js 
	DESTINATION Default/www
	COMPONENT Default)

#install(FILES *.service DESTINATION Default COMPONENT Default)

servicefile()
