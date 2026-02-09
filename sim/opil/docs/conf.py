# Configuration file for the Sphinx documentation builder.
#
# For the full list of built-in configuration values, see the documentation:
# https://www.sphinx-doc.org/en/master/usage/configuration.html

# -- Project information -----------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#project-information

project = 'OPiL'
copyright = '2025, LRS'
author = 'LRS'
release = 'v0.2'

# -- General configuration ---------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#general-configuration

extensions = ['sphinx.ext.napoleon',
              'sphinx_subfigure',
              'sphinx_rtd_theme']

templates_path = ['_templates']
exclude_patterns = ['_build', 'Thumbs.db', '.DS_Store']

numfig = True


# -- Options for HTML output -------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#options-for-html-output

html_theme = 'sphinx_rtd_theme'
html_static_path = ['_static']

rst_epilog = """
.. |simif_h| replace:: `simif.h <https://gitlab.rhrk.uni-kl.de/lrs/opil/-/blob/master/simif/simif.h>`__
.. |simif_c| replace:: `simif.c <https://gitlab.rhrk.uni-kl.de/lrs/opil/-/blob/master/simif/simif.c>`__
.. |ctlrif_h| replace:: `ctlrif.h <https://gitlab.rhrk.uni-kl.de/lrs/opil/-/blob/master/ctlrif/ctlrif.h>`__
.. |ctlrif_c| replace:: `ctlrif.h <https://gitlab.rhrk.uni-kl.de/lrs/opil/-/blob/master/ctlrif/ctlrif.c>`__
.. |opilhost_h| replace:: `opilhost.h <https://gitlab.rhrk.uni-kl.de/lrs/opil/-/blob/master/opilhost.h>`__
.. |opilhost_c| replace:: `opilhost.c <https://gitlab.rhrk.uni-kl.de/lrs/opil/-/blob/master/opilhost.c>`__
.. |opiltarget_h| replace:: `opiltarget.h <https://gitlab.rhrk.uni-kl.de/lrs/opil/-/blob/master/opiltarget.h>`__
.. |opiltarget_c| replace:: `opiltarget.c <https://gitlab.rhrk.uni-kl.de/lrs/opil/-/blob/master/opiltarget.c>`__
.. |buck_example_main_c| replace:: `main.c <https://gitlab.rhrk.uni-kl.de/lrs/opil/-/blob/master/examples/buck/src/main.c>`__
.. |buck_example_stypes_h| replace:: `stypes.h <https://gitlab.rhrk.uni-kl.de/lrs/opil/-/blob/master/examples/buck/stypes.h>`__
.. |buckcontrol_h| replace:: `buckcontrol.h <https://gitlab.rhrk.uni-kl.de/lrs/opil/-/blob/master/examples/buck/src/buckcontrol.h>`__
.. |buckcontrol_c| replace:: `buckcontrol.h <https://gitlab.rhrk.uni-kl.de/lrs/opil/-/blob/master/examples/buck/src/buckcontrol.c>`__

"""
