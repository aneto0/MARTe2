Name: 		%{rpm_id}
Version:    %{rpm_version}
Release:    %{?rpm_release:%{rpm_release}}%{!?rpm_release:1}%{?rpm_custom_dist:.%{rpm_custom_dist}}%{!?rpm_custom_dist:%{dist}}
Summary:	%{rpm_name} package
Source0:	%{rpm_id}-%{rpm_version}.tar.gz
License:	%{rpm_license}

BuildArch:    %{rpm_arch}
AutoReq:      no

Requires:   %{rpm_requires}

# Turn off the brp-python-bytecompile automagic otherwise it will clash with QA helper python scripts!
%define _python_bytecompile_errors_terminate_build 0
# Again the rpmbuild will compile any python files it finds and later complain that the .pyc and .pyo are not packaged
%define _unpackaged_files_terminate_build 0

BuildRequires:   %{rpm_build_requires}

#If rpm_conflicts not defined force its value to be zero so that the if is not evaluated
%if %{?rpm_conflicts:1}%{!?rpm_conflicts:0}
Conflicts: %{rpm_conflicts}
%endif

%description
%{rpm_name} package

%if %{?rpm_devel_id:1}%{!?rpm_devel_id:0}
%package -n %{rpm_devel_id}
Summary:	%{rpm_name} devel package

%if %{?rpm_devel_requires:1}%{!?rpm_devel_requires:0}
Requires: %{rpm_devel_requires}
%endif

%description -n %{rpm_devel_id}
%{rpm_name} package - devel

%endif

%prep
%setup -q

%build
%define debug_package %{nil}
%if %{?rpm_project_build:1}%{!?rpm_project_build:0}
. %{rpm_project_build}
%else
if [ -f Makefile.gcc ]; then
make -f Makefile.gcc TARGET=%{rpm_compile_target} %{rpm_build_options} %{rpm_build_extra_args}
else
make -f Makefile.%{rpm_compile_target} %{rpm_build_options} %{rpm_build_extra_args}
fi
%endif

%install
#Allow to run a project specific install script
%if %{?rpm_project_install:1}%{!?rpm_project_install:0}
. %{rpm_project_install}
%else
mkdir -p %{buildroot}/%{rpm_top_dir}/Bin
%if %{?rpm_bin_list:1}%{!?rpm_bin_list:0}
if [[ "%{rpm_bin_list}" != "none" ]]; then 
for exec_file in %{rpm_bin_list}
do
find %{build_dir} -iname "$exec_file" | xargs -I found_file install -m 0755 found_file %{buildroot}/%{rpm_top_dir}/Bin/
done
fi
%else
#Look for all the .ex and install them in the Bin folder
#consider only .ex and .sh by default
find %{build_dir} -executable -type f | grep '\.ex\|\.sh' | xargs -I found_file install -m 0755 found_file %{buildroot}/%{rpm_top_dir}/Bin/
%endif

mkdir -p %{buildroot}/%{rpm_top_dir}/Lib
%if %{?rpm_lib_list:1}%{!?rpm_lib_list:0}
if [[ "%{rpm_lib_list}" != "none" ]]; then 
for lib_file in %{rpm_lib_list}
do
find %{build_dir} -iname "$lib_file" | xargs -I found_file cp found_file %{buildroot}/%{rpm_top_dir}/Lib/
done
fi
%else
#Look for all the .a and .so and install them in the Lib folder
find %{build_dir} -iname "*.a" | xargs -I found_file cp found_file %{buildroot}/%{rpm_top_dir}/Lib/
find %{build_dir} -iname "*.so" | xargs -I found_file cp found_file %{buildroot}/%{rpm_top_dir}/Lib/
%endif 

%if %{?rpm_other_folders:1}%{!?rpm_devel_folders:0}
#Try to copy other user defined folders (e.g. Resources, Configurations, ...)
for other_folder in %{rpm_other_folders}
do
test -e $other_folder && cp -RL --parents $other_folder %{buildroot}/%{rpm_top_dir}
done
%endif

%if %{?rpm_other_devel_folders:1}%{!?rpm_other_devel_folders:0}
#Try to copy other user defined devel folders (e.g. MakeDefaults, Resources, ...)
for other_folder in %{rpm_other_devel_folders}
do
test -e $other_folder && cp -RL --parents $other_folder %{buildroot}/%{rpm_top_dir}
done
%endif

#Create the profile.d information
mkdir -p %{buildroot}/etc/profile.d
echo 'export %{rpm_name}_DIR=%{rpm_top_dir}' > %{buildroot}/etc/profile.d/%{rpm_id}.sh
echo 'export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$%{rpm_name}_DIR/Lib' >> %{buildroot}/etc/profile.d/%{rpm_id}.sh
echo 'export PATH=$PATH:$%{rpm_name}_DIR/Bin' >> %{buildroot}/etc/profile.d/%{rpm_id}.sh
cp %{buildroot}/etc/profile.d/%{rpm_id}.sh %{buildroot}/etc/profile.d/%{rpm_id}.csh

#Copy all .h files to an include folder
mkdir -p %{buildroot}/%{rpm_top_dir}/Include

%if %{?rpm_devel_list:1}%{!?rpm_devel_list:0}
if [[ "%{rpm_devel_list}" != "none" ]]; then 
for devel_file in %{rpm_devel_list}
do
find %{build_dir} -iname "$devel_file" | xargs -I found_file cp found_file %{buildroot}/%{rpm_top_dir}/Include/
find $src_dir -iname "$devel_file" | xargs -I found_file cp --parent found_file %{buildroot}/%{rpm_top_dir}
done
fi
%else
for src_dir in %{rpm_src_dir}
do
cd $src_dir
find . -iname "*.h" | xargs -I found_file cp found_file %{buildroot}/%{rpm_top_dir}/Include
cd -
#Copy the Source so to allow includes where all the path are relative to the original folder (e.g. Source/BareMetal/L0Types)
find $src_dir -iname "*.h" | xargs -I found_file cp --parent found_file %{buildroot}/%{rpm_top_dir}
done
%endif

%endif

#List all the files to be added to the file-list section
current_path=`pwd`
cd %{buildroot}
expattern="Include\/"
for src_dir in %{rpm_src_dir}
do
expattern=$expattern\\\|$src_dir\/
done
#Exclude also the other devel folder from going to the core rpms
for other_folder in %{rpm_other_devel_folders}
do
expattern=$expattern\\\|$other_folder\/
done

find . -type f -printf "\"/%%P\"\n" | grep -v $expattern > $current_path/file-lists
for src_dir in %{rpm_src_dir}
do
echo %{rpm_top_dir}/$src_dir >> $current_path/file-lists-devel
done
echo %{rpm_top_dir}/Include >> $current_path/file-lists-devel

%if %{?rpm_other_devel_folders:1}%{!?rpm_other_devel_folders:0}
#Add the other devel folders to the file-lists-devel
for other_folder in %{rpm_other_devel_folders}
do
#Test if the $other_folder variable is not empty (because the for is still executed if rpm_other_devel_folders is not defined
test -n "$other_folder" && echo %{rpm_top_dir}/$other_folder >> $current_path/file-lists-devel
done
%endif

cd -
%files -f file-lists

%if %{?rpm_devel_id:1}%{!?rpm_devel_id:0}
%files -n %{rpm_devel_id} -f file-lists-devel
%endif

%changelog

%post
#Note that this source command would not work as the rpm is installed as part of a different session
#source /etc/profile.d/%{rpm_id}.sh
echo 'To update the system environment variables please login again or execute "source /etc/profile.d/%{rpm_id}.sh"'

#Allow to run a project specific post install script
%if %{?rpm_project_post:1}%{!?rpm_project_post:0}
. %{rpm_project_post}
%endif

