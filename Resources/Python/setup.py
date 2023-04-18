from setuptools import setup

setup(
    name='MARTe',
    version='0.1.0',    
    description='Python MARTe support libraries',
    url='https://vcis-gitlab.f4e.europa.eu/aneto/MARTe2/',
    author='Andre Neto',
    author_email='Andre.Neto@f4e.europa.eu',
    license='EUPL',
    packages=['MARTe'],
    package_dir={'MARTe': 'Build/MARTe'},
    install_requires=[],

    classifiers=[
        'Development Status :: 4 - Beta',
        'Intended Audience :: Science/Research',
        'License :: OSI Approved :: European Union Public Licence 1.2 (EUPL 1.2)',
        'Operating System :: POSIX :: Linux',        
        'Operating System :: Microsoft :: Windows',
        'Programming Language :: Python :: 2.7',
        'Programming Language :: Python :: 3'
    ],
)
