set(INCLUDE_LIST_LOW
    "${EXAMPLE_INCLUDE_ROOT}/Model.hpp"
    "${EXAMPLE_INCLUDE_ROOT}/ModelViewer.hpp"
    "${EXAMPLE_INCLUDE_ROOT}/Cloth.hpp"
    )

set(PATH_INCLUDE "${EXAMPLE_INCLUDE_ROOT}/Paths.hpp")
configure_file("${EXAMPLE_INCLUDE_ROOT}/Paths.hpp.in" ${PATH_INCLUDE})

set(INCLUDE_LIST
    ${INCLUDE_LIST_LOW}
    ${PATH_INCLUDE}
    PARENT_SCOPE)
