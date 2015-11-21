#!/bin/sh

base=`dirname $0`
$base/cr $1 >$base/e.$1 && echo plot \'$base/e.$1\'|gnuplot -p
