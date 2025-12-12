Name:           marte2-python
Version:        %{rpm_version}
Release:        1%{?dist}
Summary:        MARTe2 Python package
License:        EUPL

%global python_v_sitelib %{_prefix}/lib/python%{python_ver}/site-packages

Source0:        MARTe-%{version}-py3-none-any.whl

BuildArch:      noarch

BuildRequires:  python%{python_ver}-devel
BuildRequires:  python%{python_ver}-pip
Requires:       python%{python_ver}
Requires:       %{pyparsing_dep}


%description
MARTe2 Python package distributed as a Python wheel.

%prep
# No source extraction needed
:

%build
# Nothing to build
:

%install
pip%{python_ver} install \
    --no-deps \
    --root %{buildroot} \
    --prefix /usr \
    %{SOURCE0}

%files
%{python_v_sitelib}/MARTe*

