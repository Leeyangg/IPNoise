#!/usr/bin/env perl

package core::object::products::main;

use strict;
use warnings;

use core::templates::perl::generic;

use core::object::object::main;
use base 'core::object::object::main';

use utf8;
use Encode;

##
## render
##
## >> view name
##
## << html
##
sub render
{
    my $self = shift;
    my $view = shift;
    my $ret  = '';

    my $tpl_path = $self->getTemplatePath(
        view        => $view,
        embedded    => 0,
        package     => __PACKAGE__
    );

    if (-r $tpl_path){
        $ret = $self->processTemplate($tpl_path);
    } else {
        ## trying SUPER
        $ret = $self->SUPER::render($view);
    }

    return $ret;
}

##
## renderEmbedded
##
## >> view name
##
## << html
##
sub renderEmbedded
{
    my $self = shift;
    my $view = shift;
    my $ret  = '';

    my $tpl_path = $self->getTemplatePath(
        view        => $view,
        embedded    => 1,
        package     => __PACKAGE__
    );

    if (-r $tpl_path){
        $ret = $self->processTemplate($tpl_path);
    } else {
        ## trying SUPER
        $ret = $self->SUPER::renderEmbedded($view);
    }

    return $ret;
}

sub getNavName
{
    my $self = shift;
    return 'раздел '.$self->{objects}->{title};
}

sub getProductsCount
{
    my $self = shift;
    my $ret  = 0;

    ## TODO optimize me
    ## 20130724 rchechnev

    my $product_els = $self->getNeighsByType("product");
    $ret = scalar(@$product_els);

    return $ret;
}

##
## getTitle
##
## << Object title
##
sub getTitle
{
    my $self = shift;
    my $ret  = $self->{objects}->{title};
    return $ret;
}

1;

