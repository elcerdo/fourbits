#!/bin/bash

set -u

function embed_framework {
framework_name="${1:?"provide framework name"}"
echo "preparing framework ${framework_name}"
framework_dir="${framework_name}.framework"
framework_orig="$(find "/Library/Frameworks" -type d -name "${framework_dir}")"
rm -fr "${framework_dir}" && cp -r "${framework_orig}" "${framework_dir}"
install_name_tool -id "@executable_path/${framework_dir}/Versions/4/${framework_name}" "${framework_dir}/Versions/4/${framework_name}"
install_name_tool -id "@executable_path/${framework_dir}/Versions/4.0/${framework_name}" "${framework_dir}/Versions/4.0/${framework_name}"
install_name_tool -id "@executable_path/${framework_dir}/Versions/Current/${framework_name}" "${framework_dir}/Versions/Current/${framework_name}"
}

embed_framework "QtCore"
embed_framework "QtGui"
