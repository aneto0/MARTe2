Name: 		DEF_NAME
Version:    DEF_VERSION
Release:    1%{?dist}
Summary:	MARTe2 RPM package
Source0:	DEF_NAME-DEF_VERSION.tar.gz
License:	FIXME

%description
MARTe2 RPM package

%prep
%setup -q

%build
export ONLY_MARTE_CORE=1
make -f Makefile.x86-linux
unset ONLY_MARTE_CORE

%install
mkdir -p %{buildroot}/usr/bin
install -m 0755 Build/x86-linux/App/MARTeApp.ex %{buildroot}/usr/bin/
mkdir -p %{buildroot}/usr/lib
cp -a Build/x86-linux/Core/MARTe2.so %{buildroot}/usr/lib/
cp -a Build/x86-linux/Core/libMARTe2.so %{buildroot}/usr/lib/

%files
/usr/bin/MARTeApp.ex
/usr/lib/MARTe2.so
/usr/lib/libMARTe2.so

%changelog

