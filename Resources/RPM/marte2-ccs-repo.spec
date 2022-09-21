Name:       %{rpm_id}
Version:    %{rpm_version}
Release:    %{?rpm_release:%{rpm_release}}%{!?rpm_release:1}%{?rpm_custom_dist:.%{rpm_custom_dist}}%{!?rpm_custom_dist:%{dist}}
Summary:    MARTe2 CODAC Core System yum repository
License:    EUPL
Source0:	%{rpm_id}-%{rpm_version}.tar.gz

AutoReq:      no

%description
MARTe2 CODAC Core System components repository

%prep
%setup -q

%install
mkdir -p %{buildroot}/etc/yum.repos.d/
mkdir -p %{buildroot}/etc/pki/rpm-gpg/
cp Resources/RPM/marte2-ccs.repo %{buildroot}/etc/yum.repos.d/
cp Resources/RPM/RPM-GPG-KEY-MARTe2 %{buildroot}/etc/pki/rpm-gpg/

%files
/etc/yum.repos.d/marte2-ccs.repo
/etc/pki/rpm-gpg/RPM-GPG-KEY-MARTe2

%post
rpm --import /etc/pki/rpm-gpg/RPM-GPG-KEY-MARTe2
#Get current CODAC version
CODAC_MAJOR=`codac-version | awk '{print substr ($0, 0, 1)}'`
CODAC_MINOR=`codac-version | awk '{print substr ($0, 3, 1)}'`
CODAC_MINOR_MINOR=`codac-version | awk '{print substr ($0, 5, 1)}'`
CODAC_CCS="ccs$CODAC_MAJOR$CODAC_MINOR$CODAC_MINOR_MINOR"
sed -i "s/CCS_VERSION/$CODAC_CCS/g" /etc/yum.repos.d/marte2-ccs.repo

