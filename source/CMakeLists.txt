set(GEN_PATH ${SOURCE}/gen)

FILE(GLOB_RECURSE PLATFORM_FILE_LIST ${INCLUDE}/tlibc/platform/*.h ${SOURCE}/platform/*.c)
source_group("platform" FILES ${PLATFORM_FILE_LIST})

FILE(GLOB_RECURSE CORE_FILE_LIST ${INCLUDE}/tlibc/core/*.h ${SOURCE}/core/*.c ${SOURCE}/core/*.h)
source_group("core" FILES ${CORE_FILE_LIST})
SET(FINAL_TARGET_FILE
	${PROTOCOL_FILE_LIST} ${PLATFORM_FILE_LIST} ${CORE_FILE_LIST} 
	${RE2C_XML_FILE} ${RE2C_XML_HEADER} ${RE2C_XML_OUTPUTS}
)

set(PROTOCOL_FILE_LIST
	${INCLUDE}/tlibc/protocol/tlibc_abstract_reader.h
	${SOURCE}/protocol/tlibc_abstract_reader.c

	${INCLUDE}/tlibc/protocol/tlibc_abstract_writer.h
	${SOURCE}/protocol/tlibc_abstract_writer.c

	${INCLUDE}/tlibc/protocol/tlibc_binary_reader.h
	${SOURCE}/protocol/tlibc_binary_reader.c

	${INCLUDE}/tlibc/protocol/tlibc_binary_writer.h
	${SOURCE}/protocol/tlibc_binary_writer.c

	${INCLUDE}/tlibc/protocol/tlibc_compact_reader.h
	${SOURCE}/protocol/tlibc_compact_reader.c

	${INCLUDE}/tlibc/protocol/tlibc_compact_writer.h
	${SOURCE}/protocol/tlibc_compact_writer.c
)
source_group("protocol" FILES ${PROTOCOL_FILE_LIST})
list(APPEND FINAL_TARGET_FILE ${PROTOCOL_FILE_LIST})


set(RE2C_XML_FILE ${SOURCE}/protocol/tlibc_xml_reader_l.re)
set(RE2C_XML_HEADER ${GEN_PATH}/tlibc_xml_reader_l.h)
set(RE2C_XML_OUTPUTS ${GEN_PATH}/tlibc_xml_reader_l.c)
if(WITH_RE2C)
	add_custom_command(OUTPUT ${RE2C_XML_HEADER} ${RE2C_XML_OUTPUTS} 
		COMMAND re2c -c -F -t${RE2C_XML_HEADER} -o${RE2C_XML_OUTPUTS} ${RE2C_XML_FILE}
	)
endif(WITH_RE2C)
source_group("protocol" FILES ${RE2C_XML_FILE} ${RE2C_XML_HEADER} ${RE2C_XML_OUTPUTS})
list(APPEND FINAL_TARGET_FILE ${RE2C_XML_FILE} ${RE2C_XML_HEADER} ${RE2C_XML_OUTPUTS})


if(ZLIB_FOUND)
	set(RE2C_XLSX_WORKBOOK_FILE ${SOURCE}/protocol/tlibc_xlsx_reader_workbook_l.re)
	set(RE2C_XLSX_WORKBOOK_HEADER ${GEN_PATH}/tlibc_xlsx_reader_workbook_l.h)
	set(RE2C_XLSX_WORKBOOK_OUTPUTS ${GEN_PATH}/tlibc_xlsx_reader_workbook_l.c)

	set(RE2C_XLSX_WORKBOOK_RELS_FILE ${SOURCE}/protocol/tlibc_xlsx_reader_workbook_rels_l.re)
	set(RE2C_XLSX_WORKBOOK_RELS_HEADER ${GEN_PATH}/tlibc_xlsx_reader_workbook_rels_l.h)
	set(RE2C_XLSX_WORKBOOK_RELS_OUTPUTS ${GEN_PATH}/tlibc_xlsx_reader_workbook_rels_l.c)

	set(RE2C_XLSX_SHAREDSTRING_FILE ${SOURCE}/protocol/tlibc_xlsx_reader_sharedstring_l.re)
	set(RE2C_XLSX_SHAREDSTRING_HEADER ${GEN_PATH}/tlibc_xlsx_reader_sharedstring_l.h)
	set(RE2C_XLSX_SHAREDSTRING_OUTPUTS ${GEN_PATH}/tlibc_xlsx_reader_sharedstring_l.c)

	set(RE2C_XLSX_FILE ${SOURCE}/protocol/tlibc_xlsx_reader_l.re)
	set(RE2C_XLSX_HEADER ${GEN_PATH}/tlibc_xlsx_reader_l.h)
	set(RE2C_XLSX_OUTPUTS ${GEN_PATH}/tlibc_xlsx_reader_l.c)
	if(WITH_RE2C)
		add_custom_command(OUTPUT ${RE2C_XLSX_SHAREDSTRING_HEADER} ${RE2C_XLSX_SHAREDSTRING_OUTPUTS} ${RE2C_XLSX_HEADER} ${RE2C_XLSX_OUTPUTS} ${RE2C_XLSX_WORKBOOK_RELS_HEADER} ${RE2C_XLSX_WORKBOOK_RELS_OUTPUTS} ${RE2C_XLSX_WORKBOOK_HEADER} ${RE2C_XLSX_WORKBOOK_OUTPUTS}
			${RE2C_XLSX_HEADER} ${RE2C_XLSX_OUTPUTS} 
			${RE2C_XLSX_WORKBOOK_HEADER} ${RE2C_XLSX_WORKBOOK_OUTPUTS}
			${RE2C_XLSX_WORKBOOK_RELS_HEADER} ${RE2C_XLSX_WORKBOOK_RELS_OUTPUTS}
			${RE2C_XLSX_SHAREDSTRING_HEADER} ${RE2C_XLSX_SHAREDSTRING_OUTPUTS}

			COMMAND re2c --case-insensitive -c -t${RE2C_XLSX_HEADER} -o${RE2C_XLSX_OUTPUTS} ${RE2C_XLSX_FILE}
			COMMAND re2c --case-insensitive -c -t${RE2C_XLSX_WORKBOOK_HEADER} -o${RE2C_XLSX_WORKBOOK_OUTPUTS} ${RE2C_XLSX_WORKBOOK_FILE}
			COMMAND re2c --case-insensitive -c -t${RE2C_XLSX_WORKBOOK_RELS_HEADER} -o${RE2C_XLSX_WORKBOOK_RELS_OUTPUTS} ${RE2C_XLSX_WORKBOOK_RELS_FILE}
			COMMAND re2c --case-insensitive -c -t${RE2C_XLSX_SHAREDSTRING_HEADER} -o${RE2C_XLSX_SHAREDSTRING_OUTPUTS} ${RE2C_XLSX_SHAREDSTRING_FILE}
		)
	endif(WITH_RE2C)
	INCLUDE_DIRECTORIES(${ZLIB_INCLUDE_DIRS})
	source_group("protocol" FILES
		${RE2C_XLSX_FILE} ${RE2C_XLSX_HEADER} ${RE2C_XLSX_OUTPUTS}
		${RE2C_XLSX_WORKBOOK_FILE} ${RE2C_XLSX_WORKBOOK_HEADER} ${RE2C_XLSX_WORKBOOK_OUTPUTS}
		${RE2C_XLSX_WORKBOOK_RELS_FILE} ${RE2C_XLSX_WORKBOOK_RELS_HEADER} ${RE2C_XLSX_WORKBOOK_RELS_OUTPUTS}
		${RE2C_XLSX_SHAREDSTRING_FILE} ${RE2C_XLSX_SHAREDSTRING_HEADER} ${RE2C_XLSX_SHAREDSTRING_OUTPUTS}
	)
	list(APPEND FINAL_TARGET_FILE
	${RE2C_XLSX_FILE} ${RE2C_XLSX_HEADER} ${RE2C_XLSX_OUTPUTS}
	${RE2C_XLSX_WORKBOOK_FILE} ${RE2C_XLSX_WORKBOOK_HEADER} ${RE2C_XLSX_WORKBOOK_OUTPUTS}
	${RE2C_XLSX_WORKBOOK_RELS_FILE} ${RE2C_XLSX_WORKBOOK_RELS_HEADER} ${RE2C_XLSX_WORKBOOK_RELS_OUTPUTS}
	${RE2C_XLSX_SHAREDSTRING_FILE} ${RE2C_XLSX_SHAREDSTRING_HEADER} ${RE2C_XLSX_SHAREDSTRING_OUTPUTS}
	)
else()
	list(REMOVE_ITEM FINAL_TARGET_FILE ${SOURCE}/core/tlibc_unzip.c)
	message("XLSX is not available.")
endif(ZLIB_FOUND)

if(MYSQL_FOUND)
	set(MYSQL_FILE_LIST 
		${INCLUDE}/tlibc/protocol/tlibc_bind_reader.h
		${SOURCE}/protocol/tlibc_bind_reader.c
	)
	source_group("protocol" FILES ${MYSQL_FILE_LIST})
	INCLUDE_DIRECTORIES(${MYSQL_INCLUDE_DIR})
	list(APPEND FINAL_TARGET_FILE ${MYSQL_FILE_LIST})
else()
	message("Mysql is not available.")
endif(MYSQL_FOUND)



INCLUDE_DIRECTORIES(
	${INCLUDE}
	${GEN_PATH}
	.
	)

ADD_LIBRARY(${PNAME} ${FINAL_TARGET_FILE})
SET_TARGET_PROPERTIES(${PNAME} PROPERTIES OUTPUT_NAME tlibc)

INSTALL(TARGETS ${PNAME} ARCHIVE DESTINATION "lib")
