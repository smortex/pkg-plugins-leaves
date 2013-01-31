## General Information

The *leaves* pkg plugin is used to display leaf packages (packages that have no
other packages depending on them).

## How to build the plugin?

In order to build the plugin enter into the plugin's directory and run `make(1)`, e.g.:

	$ cd /path/to/pkg-plugins-leaves
	$ make

Once the plugin is built you can install it using the following command:

	$ make install

The plugin will be installed as a shared library in `${PREFIX}/lib/pkg/leaves.so`

## Configuring the plugin

There is no configuration options available for this plugin.

## Using the plugin

Add the plugin to `${PREFIX}/etc/pkg.conf`:

	PLUGINS:
	 - leaves

Then check that it is recongnized and loaded by pkgng by executing the `pkg
plugins` command:

	$ pkg plugins
	NAME       DESC                                          VERSION   
	leaves     A plugin for querying leaf packages           1.0.0     

If the plugin shows up correctly then you are good to go:

	$ pkg leaves
	bacula-client-5.2.12           Network backup solution (client)
	cdiff-1.5                      Diff readability enhancer for color terminals
	portmaster-3.14_8              Manage your ports without external databases or languages
	portshaker-config-1.0.7        Simple configuration manager for portshaker
	puppet-3.0.2                   A configuration management framework written in Ruby
	rsync-3.0.9_3                  A network file distribution/synchronization utility
	sudo-1.8.6.p5                  Allow others to run commands as root
	tig-1.1                        Text-mode interface for git
	tree-1.6.0                     Display a tree-view of directories with optional color or HTML output
	vim-lite-7.3.669               Vi "workalike", with many additional features (Lite package)
	zsh-5.0.2                      The Z shell

