# usage `bash change_coherency_policy.sh file_name line_number policy`
sed -i "$2s/.*/variant = $3/" $1