Name: 		DEF_NAME
Version:    DEF_VERSION
Release:    1%{?dist}
Summary:	MARTe2 RPM package
Source0:	DEF_NAME-DEF_VERSION.tar.gz
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
export ONLY_MARTE_CORE=1
make -f Makefile.x86-linux
unset ONLY_MARTE_CORE

%install
## Probably would be better to install in /opt/marte2 and organise the rest through links
mkdir -p %{buildroot}/usr/bin
install -m 0755 Build/x86-linux/App/MARTeApp.ex %{buildroot}/usr/bin/
mkdir -p %{buildroot}/usr/lib
cp -a Build/x86-linux/Core/MARTe2.so %{buildroot}/usr/lib/
cp -a Build/x86-linux/Core/libMARTe2.so %{buildroot}/usr/lib/
mkdir -p %{buildroot}/opt/marte2/include
find Source -iname "*.h" | xargs -If cp --parent f %{buildroot}/opt/marte2/include

%files
/usr/bin/MARTeApp.ex
/usr/lib/MARTe2.so
/usr/lib/libMARTe2.so

%files -n %subpackage devel
/opt/marte2/include

%changelog

