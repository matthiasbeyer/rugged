/*
 * The MIT License
 *
 * Copyright (c) 2015 GitHub, Inc
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "rugged.h"

extern VALUE rb_cRuggedRefdbBackend;
VALUE rb_cRuggedRefdbBackendCustom;

typedef struct {
	git_refdb_backend parent;
	VALUE self;
} rugged_refdb_backend_custom;

static int rugged_refdb_backend_custom__exists(
	int *exists,
	git_refdb_backend *_backend,
	const char *ref_name)
{
	rugged_refdb_backend_custom *backend = (rugged_refdb_backend_custom *)_backend;

	// TODO: Proper exception handling
	*exists = RTEST(rb_funcall(backend->self, rb_intern("exists"), 1, rb_str_new2(ref_name)));

	return GIT_OK;
}

static int rugged_refdb_backend_custom__compress(git_refdb_backend *_backend)
{
	rugged_refdb_backend_custom *backend = (rugged_refdb_backend_custom *)_backend;

	// TODO: Proper exception handling
	rb_funcall(backend->self, rb_intern("compress"), 0);

	return GIT_OK;
}

static void rb_git_refdb_backend__free(git_refdb_backend *backend)
{
	if (backend) backend->free(backend);
}

static VALUE rb_git_refdb_backend_custom_new(VALUE self, VALUE rb_repo)
{
	rugged_refdb_backend_custom *backend;

	backend = xcalloc(1, sizeof(rugged_refdb_backend_custom));

	backend->parent.exists = &rugged_refdb_backend_custom__exists;
	backend->parent.compress = &rugged_refdb_backend_custom__compress;
	backend->parent.free = xfree;

	backend->self = Data_Wrap_Struct(self, NULL, rb_git_refdb_backend__free, backend);
	return backend->self;
}

void Init_rugged_refdb_backend_custom(void)
{
	rb_cRuggedRefdbBackendCustom = rb_define_class_under(rb_cRuggedRefdbBackend, "Custom", rb_cRuggedRefdbBackend);

	rb_define_singleton_method(rb_cRuggedRefdbBackendCustom, "new", rb_git_refdb_backend_custom_new, 1);
}
