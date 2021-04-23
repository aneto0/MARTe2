Name: 		%{rpm_name}
Version:    %{rpm_version}
Release:    %{?rpm_release:%{rpm_release}}%{!?rpm_release:1}
Requires:   %{rpm_requires}
BuildRequires:   %{rpm_build_requires}
Summary:	MARTe2 RPM package
Source0:	%{rpm_name}-%{rpm_version}.tar.gz
License:	FIXME

BuildArch:    x86_64
AutoReq:      no

%description
MARTe2 RPM package

%package -n %subpackage devel
Summary:	MARTe2 RPM devel package
%provides_self %subpackage devel

%description -n %subpackage devel
MARTe2 RPM package - devel


%prep
%setup -q

%build
unset MARTe2_DIR
make -f Makefile.x86-linux %{rpm_build_options}

%install
mkdir -p %{buildroot}/%{rpm_inst_dir}/bin
find . -iname "*.ex" | xargs -Ifanculo install -m 0755 fanculo %{buildroot}/%{rpm_inst_dir}/bin
mkdir -p %{buildroot}/%{rpm_inst_dir}/lib
find . -iname "*.so" | xargs -Ifanculo cp fanculo %{buildroot}/%{rpm_inst_dir}/lib
mkdir -p %{buildroot}/%{rpm_inst_dir}/include
find . -iname "*.h" | xargs -Ifanculo cp --parents fanculo %{buildroot}/%{rpm_inst_dir}/include
mkdir -p %{buildroot}/%{rpm_sys_conf_dir}/ld.so.conf.d
echo /%{rpm_inst_dir}/lib > %{buildroot}/%{rpm_sys_conf_dir}/ld.so.conf.d/%{rpm_name}-%{rpm_version}.conf
mkdir -p %{buildroot}/%{rpm_sys_conf_dir}/env.d
echo export MARTe2_DIR=/%{rpm_inst_dir}/ > %{buildroot}/%{rpm_sys_conf_dir}/env.d/%{rpm_name}-%{rpm_version}.conf

%files
/%{rpm_sys_conf_dir}/ld.so.conf.d/%{rpm_name}-%{rpm_version}.conf
/%{rpm_sys_conf_dir}/env.d/%{rpm_name}-%{rpm_version}.conf
/%{rpm_inst_dir}/bin
/%{rpm_inst_dir}/lib
/%{rpm_inst_dir}/lib

%files -n %subpackage devel
/%{rpm_inst_dir}/include

%changelog

