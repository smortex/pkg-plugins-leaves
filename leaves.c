/*-
 * Copyright (c) 2013 Romain Tartiere <romain@FreeBSD.org>
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer
 *    in this position and unchanged.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR(S) ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR(S) BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <assert.h>
#include <sqlite3.h>
#include <string.h>
#include <sysexits.h>
#include <unistd.h>

#include <pkg.h>

static char myname[] = "leaves";
static char myversion[] = "1.0.0";
static char mydescription[] = "A plugin for querying leaf packages";

static struct pkg_plugin *self;

int
pkg_plugin_init(struct pkg_plugin *p)
{
	self = p;


	pkg_plugin_set(p, PKG_PLUGIN_NAME, myname);
	pkg_plugin_set(p, PKG_PLUGIN_VERSION, myversion);
	pkg_plugin_set(p, PKG_PLUGIN_DESC, mydescription);

	return (EPKG_OK);
}

int
pkg_plugin_shutdown(struct pkg_plugin *p __unused)
{
	/* nothing to be done here */

	return (EPKG_OK);
}

void
plugin_leaves_usage(void)
{
	fprintf(stderr, "usage: pkg %s [-q]\n\n", myname);
	fprintf(stderr, "%s\n", mydescription);
}

int
plugin_leaves_callback(int argc, char **argv)
{
	struct pkgdb *db = NULL;
	struct pkgdb_it *it;
	struct pkg *pkg = NULL;
	int n;
	int ret = EPKG_OK;
	const char *name, *version, *comment;
	bool quiet = false;
	char ch;

	while ((ch = getopt(argc, argv, "q")) != -1) {
		switch (ch) {
		case 'q':
			quiet = true;
			break;
		default:
			plugin_leaves_usage();
			return (EX_USAGE);
			break;
		}
	}

	argc -= optind;
	argv += optind;

	if (argc > 0) {
		plugin_leaves_usage();
		return (EX_USAGE);
	}

	if (pkgdb_open(&db, PKGDB_DEFAULT) != EPKG_OK) {
	    return (EX_IOERR);
	}

	if ((it = pkgdb_query(db, "\0", MATCH_ALL)) == NULL)
		return EX_IOERR;

	while ((ret = pkgdb_it_next(it, &pkg, PKG_LOAD_RDEPS)) == EPKG_OK) {
		pkg_get(pkg, PKG_NAME, &name, PKG_VERSION, &version, PKG_COMMENT, &comment);
		if (pkg_list_count(pkg, PKG_RDEPS) == 0) {
			printf("%s-%s ", name, version);
			if (quiet) {
				printf("\n");
			} else {
				n = strlen(name) + strlen(version) + 1;
				do {
					printf(" ");
					n++;
				} while (n < 30);
				printf("%s\n", comment);
			}
		}
	}

	pkgdb_it_free(it);

	pkgdb_close(db);

	return (EPKG_OK);
}

int
pkg_register_cmd(int idx, const char **name, const char **desc, int (**exec)(int argc, char **argv))
{
	*name = myname;
	*desc = mydescription;
	*exec = plugin_leaves_callback;

	return (EPKG_OK);
}

int
pkg_register_cmd_count (void)
{
    return 1;
}
