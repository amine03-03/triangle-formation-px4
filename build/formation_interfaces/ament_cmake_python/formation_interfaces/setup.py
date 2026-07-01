from setuptools import find_packages
from setuptools import setup

setup(
    name='formation_interfaces',
    version='0.1.0',
    packages=find_packages(
        include=('formation_interfaces', 'formation_interfaces.*')),
)
