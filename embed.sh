#!/bin/bash

set -u

function embed_framework {
local framework_name="${1:?"provide framework name"}"
echo "preparing framework ${framework_name}"
local framework_dir="${framework_name}.framework"
local framework_orig="$(find "/Library/Frameworks" -type d -name "${framework_dir}")"
rm -fr "${framework_dir}" && cp -r "${framework_orig}" "${framework_dir}"
install_name_tool -id "@executable_path/${framework_dir}/Versions/4/${framework_name}" "${framework_dir}/Versions/4/${framework_name}"
install_name_tool -id "@executable_path/${framework_dir}/Versions/4.0/${framework_name}" "${framework_dir}/Versions/4.0/${framework_name}"
install_name_tool -id "@executable_path/${framework_dir}/Versions/Current/${framework_name}" "${framework_dir}/Versions/Current/${framework_name}"
}

#embed_framework "QtCore"
#embed_framework "QtGui"

base_dir="fourbits.app/Contents/MacOS"
base_bin="${base_dir}/fourbits"
test -f "${base_bin}" || exit 23
otool -L "${base_bin}"
macdeployqt fourbits.app -no-plugins -dmg
otool -L "${base_bin}"
