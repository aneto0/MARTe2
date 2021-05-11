Name: 		%{rpm_id}
Version:    %{rpm_version}
Release:    %{?rpm_release:%{rpm_release}}%{!?rpm_release:1}%{?rpm_custom_dist:.%{rpm_custom_dist}}%{!?rpm_custom_dist:%{dist}}
Summary:	MARTe2 yum repository
License:	EUPL
Source0:	%{rpm_id}-%{rpm_version}.tar.gz

AutoReq:      no

%description
MARTe2 release repository

%prep
%setup -q

%install
mkdir -p %{buildroot}/etc/yum.repos.d/
mkdir -p %{buildroot}/etc/pki/rpm-gpg/
cp Resources/RPM/marte2.repo %{buildroot}/etc/yum.repos.d/
cp Resources/RPM/RPM-GPG-KEY-MARTe2 %{buildroot}/etc/pki/rpm-gpg/

%files
/etc/yum.repos.d/marte2.repo
/etc/pki/rpm-gpg/RPM-GPG-KEY-MARTe2

%post
#Forced to do sed because $releasever is RHEL dependent
sed -i "s/{rpm_releasever}/$RELEASEVER/g" /etc/yum.repos.d/marte2-ccs.repo
rpm --import /etc/pki/rpm-gpg/RPM-GPG-KEY-MARTe2
