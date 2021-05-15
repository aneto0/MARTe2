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

%package -n %{rpm_id}-devel
Summary:	%{rpm_name} devel package

%description -n %{rpm_id}-devel
%{rpm_name} package - devel

%prep
%setup -q

%build
make -f Makefile.%{rpm_compile_target} %{rpm_build_options} %{rpm_build_extra_args}

%install
#Look for all the .ex and install them in the Bin folder
mkdir -p %{buildroot}%{rpm_top_dir}/%{rpm_id}/Bin
find %{_builddir} -iname "*.ex" | xargs -I found_file install -m 0755 found_file %{buildroot}/%{rpm_top_dir}/%{rpm_id}/Bin/
#Look for all the .a and .so and install them in the Lib folder
mkdir -p %{buildroot}%{rpm_top_dir}/%{rpm_id}/Lib
find %{_builddir} -iname "*.a" | xargs -I found_file cp found_file %{buildroot}%{rpm_top_dir}/%{rpm_id}/Lib/
find %{_builddir} -iname "*.so" | xargs -I found_file cp found_file %{buildroot}%{rpm_top_dir}/%{rpm_id}/Lib/

#Try to copy other user defined folders (e.g. Resources, Configurations, ...)
for other_folder in %{rpm_other_folders}
do
test -e $other_folder && cp -R $other_folder %{buildroot}%{rpm_top_dir}/%{rpm_id}/
done

#Create the profile.d information
mkdir -p %{buildroot}/etc/profile.d
echo 'export %{rpm_name}_DIR=%{rpm_top_dir}/%{rpm_id}' > %{buildroot}/etc/profile.d/%{rpm_id}.sh
echo 'export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$%{rpm_name}_DIR/Lib' >> %{buildroot}/etc/profile.d/%{rpm_id}.sh
echo 'export PATH=$PATH:$%{rpm_name}_DIR/Bin' >> %{buildroot}/etc/profile.d/%{rpm_id}.sh
cp %{buildroot}/etc/profile.d/%{rpm_id}.sh %{buildroot}/etc/profile.d/%{rpm_id}.csh

#Copy all .h files to an include folder
mkdir -p %{buildroot}%{rpm_top_dir}/%{rpm_id}/Include
cd %{rpm_src_dir}
find . -iname "*.h" | xargs -I found_file cp found_file %{buildroot}%{rpm_top_dir}/%{rpm_id}/Include
cd ..
#Copy the Source so to allow includes where all the path are relative to the original folder (e.g. Source/BareMetal/L0Types)
find %{rpm_src_dir} -iname "*.h" | xargs -I found_file cp --parent found_file %{buildroot}%{rpm_top_dir}/%{rpm_id}

#Allow to run a project specific install script
%if %{?rpm_project_install:1}%{!?rpm_project_install:0}
. {rpm_project_install}
%endif

#List all the files to be added to the file-list section
current_path=`pwd`
cd %{buildroot}
find . -type f -printf "\"/%%P\"\n" | grep -v %{rpm_src_dir} | grep -v Include > $current_path/file-lists
cd -

%files -f file-lists

%files -n %{rpm_id}-devel
%{rpm_top_dir}/%{rpm_id}/%{rpm_src_dir}
%{rpm_top_dir}/%{rpm_id}/Include

%changelog

%post
#Note that this source command would not work as the rpm is installed as part of a different session
#source /etc/profile.d/%{rpm_id}.sh
echo 'To update the system environment variables please login again or execute "source /etc/profile.d/%{rpm_id}.sh"'

#Allow to run a project specific post install script
%if %{?rpm_project_post:1}%{!?rpm_project_post:0}
. {rpm_project_post}
%endif

