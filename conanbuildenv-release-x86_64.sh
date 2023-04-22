echo "echo Restoring environment" > "/home/max/cpp-projects/losshd-1.0.1/deactivate_conanbuildenv-release-x86_64.sh"
for v in 
do
    is_defined="true"
    value=$(printenv $v) || is_defined="" || true
    if [ -n "$value" ] || [ -n "$is_defined" ]
    then
        echo export "$v='$value'" >> "/home/max/cpp-projects/losshd-1.0.1/deactivate_conanbuildenv-release-x86_64.sh"
    else
        echo unset $v >> "/home/max/cpp-projects/losshd-1.0.1/deactivate_conanbuildenv-release-x86_64.sh"
    fi
done

