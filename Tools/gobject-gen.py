#!/usr/bin/env python
#
# Ross Burton <ross@burtonini.com>
# Dafydd Harries <daf@rhydd.org>
#
# PyGTK Version by:
#  Thomas Wood <thos@gnome.org>
#
# Copyright 2015 Peter Hatina
# Copyright 2009 Intel Corporation
# Copyright 2005 Ross Burton
# Copyright 2005 Dafydd Harries
#
#   This program is free software: you can redistribute it and/or modify
#   it under the terms of the GNU General Public License as published by
#   the Free Software Foundation, either version 3 of the License, or
#   (at your option) any later version.
#
#   This program is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU General Public License for more details.
#
#   You should have received a copy of the GNU General Public License
#   along with this program.  If not, see <http://www.gnu.org/licenses/>.
#
# Template text is Public Domain

import abc
import argparse
import os
import re
import sys

PACKAGE_NAME    = 'GObject Generator'
PACKAGE_VERSION = '0.1'
PACKAGE_AUTHORS = ['Peter Hatina <phatina@redhat.com>',
                   'Thomas Wood <thos@gnome.org>',
                   'Ross Burton <ross@burtonini.com>',
                   'Dafydd Harries <daf@rhydd.org>']
PACKAGE_COPYRIGHT = 'Copyright 2015 Red Hat, Inc.\n' \
                    'Copyright 2009 Intel Corporation\n' \
                    'Copyright 2005 Ross Burton, Dafydd Harries'

# ------------------------------------------------------------------------------
# H Template

h_template_header = '''\
#ifndef %(header_guard)s
#define %(header_guard)s

#include <glib-object.h>

G_BEGIN_DECLS
'''

h_template_macros = '''\
#define %(package_upper)s_TYPE_%(object_upper)s            (%(class_lower)s_get_type ())
#define %(package_upper)s_%(object_upper)s(o)              (G_TYPE_CHECK_INSTANCE_CAST  ((o), %(package_upper)s_TYPE_%(object_upper)s, %(class_camel)s))
#define %(package_upper)s_IS_%(object_upper)s(o)           (G_TYPE_CHECK_INSTANCE_TYPE  ((o), %(package_upper)s_TYPE_%(object_upper)s))
#define %(package_upper)s_%(object_upper)s_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), %(package_upper)s_TYPE_%(object_upper)s, %(class_camel)sClass))
#define %(package_upper)s_IS_%(object_upper)s_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), %(package_upper)s_TYPE_%(object_upper)s))
#define %(package_upper)s_%(object_upper)s_GET_CLASS(o)    (G_TYPE_INSTANCE_GET_CLASS ((o), %(package_upper)s_TYPE_%(object_upper)s, %(class_camel)sClass))
'''

h_template_macros_priv = '''\
#define %(package_upper)s_%(object_upper)s_GET_PRIVATE(o)  (G_TYPE_INSTANCE_GET_PRIVATE ((o), %(package_upper)s_TYPE_%(object_upper)s, %(class_camel)sPrivate))
'''

h_template_types_decls = '''\
typedef struct _%(class_camel)s        %(class_camel)s;
typedef struct _%(class_camel)sClass   %(class_camel)sClass;
typedef struct _%(class_camel)sPrivate %(class_camel)sPrivate;
'''

h_template_methods = '''\
GType %(class_lower_spacing)s%(class_lower)s_get_type (void) G_GNUC_CONST;
%(class_camel)s %(class_camel_spacing)s*%(class_lower)s_new (void);
'''

h_template_footer = '''\
G_END_DECLS

#endif /* %(header_guard)s */
'''

def make_h_template(use_priv):
    if use_priv:
        return '\n'.join([
            h_template_header,
            h_template_macros,
            h_template_types_decls,
            c_template_class_meta_def,
            c_template_class_def_priv,
            h_template_methods,
            h_template_footer])
    else:
        return '\n'.join([
            h_template_header,
            h_template_macros,
            h_template_types_decls,
            h_template_methods,
            h_template_footer])


# ------------------------------------------------------------------------------

# ------------------------------------------------------------------------------
# C Template

c_template_header = '''\
#include "%(filename)s.h"
'''

c_template_class_def_nopriv = '''\
struct _%(class_camel)s {
  %(parent_camel)s parent_instance;
};
'''

c_template_class_def_priv = '''\
struct _%(class_camel)s {
  %(parent_camel)s parent_instance;
  %(class_camel)sPrivate *priv;
};
'''

c_template_class_meta_def = '''\
struct _%(class_camel)sClass {
  %(parent_camel)sClass parent_class;
};
'''

c_template_type = '''\
G_DEFINE_TYPE_WITH_PRIVATE(%(class_camel)s, %(class_lower)s, %(parent)s);
'''

c_template_init = '''\
%(class_init)s

static void %(class_lower)s_init (%(class_camel)s *self) {
}
'''

c_template_init_priv = '''\
%(class_init)s

static void %(class_lower)s_init (%(class_camel)s *self) {
  self->priv = %(class_lower)s_get_instance_private (self);
}
'''

c_template_new = '''\
%(class_camel)s * %(class_lower)s_new (void) {
  return g_object_new (%(package_upper)s_TYPE_%(object_upper)s, NULL);
}
'''

c_template_private_decl = '''\
typedef struct _%(class_camel)sPrivate %(class_camel)sPrivate;
'''

c_template_private_def = '''\
struct _%(class_camel)sPrivate {
  /* TODO: Add members here. */
};

enum {
  NUM_PROPERTIES
};

static GParamSpec *%(class_lower)s_props[NUM_PROPERTIES];
'''

c_template_prop = '''\
static void %(class_lower)s_get_property (GObject    *object,
%(get_property_spacing)sguint       property_id,
%(get_property_spacing)sGValue     *value,
%(get_property_spacing)sGParamSpec *pspec) {
  switch (property_id) {
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
    }
}

static void %(class_lower)s_set_property (GObject      *object,
%(set_property_spacing)sguint         property_id,
%(set_property_spacing)sconst GValue *value,
%(set_property_spacing)sGParamSpec   *pspec) {
  switch (property_id) {
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
    }
}
'''

c_template_constructed = '''\
static void %(class_lower)s_constructed (GObject *object) {
  %(class_camel)s *self = %(package_upper)s_%(object_upper)s(object);

  G_OBJECT_CLASS (%(class_lower)s_parent_class)->constructed (object);
}
'''

c_template_dispose = '''\
static void %(class_lower)s_dispose (GObject *object) {
  %(class_camel)s *self = %(package_upper)s_%(object_upper)s(object);

  G_OBJECT_CLASS (%(class_lower)s_parent_class)->dispose (object);
}
'''

c_template_finalize = '''\
static void %(class_lower)s_finalize (GObject *object) {
  %(class_camel)s *self = %(package_upper)s_%(object_upper)s(object);

  G_OBJECT_CLASS (%(class_lower)s_parent_class)->finalize (object);
}
'''

def make_c_template(private=False, props=False, constructed=False,
                    dispose=False, finalize=False):
    c_template = [c_template_header]

    if private:
        c_template.append(c_template_private_def)

    c_template.append(c_template_type)

    if props:
        c_template.append(c_template_prop)

    if constructed:
        c_template.append(c_template_constructed)

    if dispose:
        c_template.append(c_template_dispose)

    if finalize:
        c_template.append(c_template_finalize)

    if private:
        c_template.append(c_template_init_priv)
    else:
        c_template.append(c_template_init)

    c_template.append(c_template_new)

    return '\n'.join(c_template)


class Generator(object):
    def __init__(self, klass, parent_class=None, include_private=False,
            include_getset=False, include_constructed=False,
            include_finalize=False, include_dispose=False,
            filename_separator=None):
        self.klass = klass

        if parent_class is None:
            parent_class = 'GObject'
        self.parent_class = parent_class

        self.private = include_private
        self.props = include_getset
        self.constructed = include_constructed
        self.finalize = include_finalize
        self.dispose = include_dispose
        self.filename_separator = filename_separator

    def make_template_data(self):
        package_upper, object_upper, class_lower = self.guess_class_params(self.klass)
        parent = self.guess_parent_params(self.parent_class)
        return self.make_template_data_with_params(
            package_upper, object_upper, class_lower, parent)

    def make_template_data_with_params(self, package_upper, object_upper,
            class_lower, parent):
        return {
            'class_camel': self.klass,
            'class_lower': class_lower,
            'package_upper': package_upper,
            'object_upper': object_upper,
            'parent': parent,
            'parent_camel': self.parent_class,
            'private': self.private,
            'props': self.props,
            'constructed': self.constructed,
            'finalize': self.finalize,
            'dispose': self.dispose,
            'filename_separator': self.filename_separator}

    def generate_sources(self, folder=None, **data):
        if not data.get('class_camel') or not data.get('class_lower'):
            raise ValueError('Class name missing')

        # If no folder name supplied, let's assume that we generate sources in
        # current working directory.
        if not folder:
            # None or empty string
            folder = os.getcwd()

        # Spacing members
        data_lower_len = len('GType')
        data_camel_len = len(data['class_camel'])
        data_lower_spacing = data_camel_len - data_lower_len if data_camel_len > data_lower_len else 0
        data_camel_spacing = data_lower_len - data_camel_len if data_lower_len > data_camel_len else 0
        data['class_lower_spacing'] = ' ' * (data_lower_spacing + 1)
        data['class_camel_spacing'] = ' ' * data_camel_spacing
        data['get_property_spacing'] = ' ' * (len(data['class_lower']) + len('_get_property ('))
        data['set_property_spacing'] = ' ' * (len(data['class_lower']) + len('_set_property ('))

        # Filename separator
        separator = data['filename_separator']
        if separator is None:
            separator = '-'

        # Filename itself
        data['filename'] = data['class_camel'].replace('_', separator)

        data['class_init'] = self.make_class_init(data).strip()
        data['header_guard'] = '__' + data['class_lower'].upper().replace('.', '_').replace('-', '_') + '_H__'

        license = self.read_license()

        hfile = os.path.join(folder, data['filename'] + '.h')
        cfile = os.path.join(folder, data['filename'] + '.c')

        if os.path.exists(hfile) or os.path.exists(cfile):
            print("Failed: file exists in folder \"%s\"" % (folder))
            return

        # Write header file.
        with open(hfile, 'w') as f:
            if license:
                f.write(license + '\n')
            f.write(make_h_template(data['private']) % data)

        # Write source file.
        with open(cfile, 'w') as f:
            if license:
                f.write(license + '\n')

            f.write(
                make_c_template(
                    data['private'],
                    data['props'],
                    data['constructed'],
                    data['dispose'],
                    data['finalize']) % data)

    def read_license(self, filename=None):
        license = ''
        if filename is None:
            filename = 'license.txt'
        if os.path.exists(filename):
            with open(filename, 'r') as fin_license:
                license = fin_license.read()
        return license

    @staticmethod
    def make_class_init(data):
        lines = ['static void %(class_lower)s_class_init (%(class_camel)sClass *klass) {']

        if data['constructed'] or data['dispose'] or data['finalize'] or data['private']:
            lines.append('  GObjectClass *gobject_class = G_OBJECT_CLASS (klass);')
            lines.append('')

        if data['props']:
            lines.append('  gobject_class->get_property = %(class_lower)s_get_property;')
            lines.append('  gobject_class->set_property = %(class_lower)s_set_property;')

        if data['constructed']:
            lines.append('  gobject_class->constructed = %(class_lower)s_constructed;')

        if data['dispose']:
            lines.append('  gobject_class->dispose = %(class_lower)s_dispose;')

        if data['finalize']:
            lines.append('  gobject_class->finalize = %(class_lower)s_finalize;')

        lines.append('')
        lines.append('  g_object_class_install_properties (gobject_class, NUM_PROPERTIES, %(class_lower)s_props);')
        lines.append('}')
        return ''.join([line % data + '\n' for line in lines])

    @staticmethod
    def guess_class_params(class_camel):
        m = re.findall('[A-Z]+[a-z]*', class_camel)
        if (m):
          s = m[0]
        else:
          s = ''

        package_upper = s.upper()

        if (m and len(m) > 1):
          s = m[1]
          i = 2
          while (i < len(m)):
            s = s + '_' + m[i]
            i = i + 1
        else:
           s = ''

        object_upper = s.upper()

        if (m):
          s = m[0]
          i = 1
          while (i < len(m)):
            s = s + '_' + m[i]
            i = i + 1
        else:
          s = ''

        class_lower = s.lower()

        return package_upper, object_upper, class_lower

    @staticmethod
    def guess_parent_params(parent_camel):
        if parent_camel == 'GObject':
            # special case GObject
            s = 'G_TYPE_OBJECT'
        else:
            m = re.findall('[A-Z]+[a-z]*', parent_camel)
            s = ''
            if (m):
                s = m[0] + '_TYPE'
                if (len(m) > 1):
                    i = 1
                    while (i < len(m)):
                        s = s + '_' + m[i]
                        i = i + 1

        return s.upper()

    @abc.abstractmethod
    def generate(self):
        pass

class CliGenerator(Generator):
    def __init__(self, klass, parent_class=None,
            include_private=False, include_getset=False,
            include_constructed=False, include_finalize=False,
            include_dispose=False, filename_separator=None):
        super(CliGenerator, self).__init__(
            klass, parent_class, include_private, include_getset,
            include_constructed, include_finalize, include_dispose,
            filename_separator)

    def generate(self, folder):
        # Skip folder argument, we generate in $PWD
        self.generate_sources(folder, **self.make_template_data())


def parse_args(args=sys.argv[1:]):
    parser = argparse.ArgumentParser()
    parser.add_argument('-c', '--class', dest='klass', help='Class name')
    parser.add_argument('-b', '--base', dest='base', help='Base class name')
    parser.add_argument('-o', '--output', dest='output', help='Output directory')
    parser.add_argument(
        '-p', '--include-private',
        dest='private', action='store_true', default=False,
        help='Include private struct')
    parser.add_argument(
        '-g', '--include-get-set',
        dest='getset', action='store_true', default=False,
        help='Include GObject property get/set')
    parser.add_argument(
        '-t', '--include-constructed',
        dest='constructed', action='store_true', default=False,
        help='Include constructed')
    parser.add_argument(
        '-f', '--include-finalize',
        dest='finalize', action='store_true', default=False,
        help='Include finalize')
    parser.add_argument(
        '-d', '--include-dispose',
        dest='dispose', action='store_true', default=False,
        help='Include dispose')
    parser.add_argument(
        '-u', '--sep-underscore',
        dest='sep_underscore', action='store_true', default=False,
        help='Use underscore as filename separator (default is dash)')
    parser.add_argument(
        '-n', '--no-sep',
        dest='sep_none', action='store_true', default=False,
        help='Use no filename separator (default is dash)')
    return parser.parse_args(args)


if __name__ == '__main__':
    options = parse_args()

    separator = None
    if options.sep_underscore:
        separator = '_'
    elif options.sep_none:
        separator = ''

    generator_cls = CliGenerator

    try:
        g = generator_cls(
            klass=options.klass,
            parent_class=options.base,
            include_private=options.private,
            include_getset=options.getset,
            include_constructed=options.constructed,
            include_finalize=options.finalize,
            include_dispose=options.dispose,
            filename_separator=separator)
        g.generate(options.output)
    except Exception as e:
        sys.stderr.write('Error: %s\n' % str(e))
