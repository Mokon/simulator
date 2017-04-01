#!/usr/bin/perl
# Copyright (C) 2017 David 'Mokon' Bond, All Rights Reserved

package TestCase;

use strict;
use warnings;

use Term::ANSIColor;

sub new {
    my (
        $class,             $num_simulations,  $general_roll,
        $risk_europe,       $risk,             $verbose,
        $stats,             $attacker_peasent, $attacker_archer,
        $attacker_infantry, $attacker_cavalry, $attacker_pikemen,
        $attacker_siege,    $defender_peasent, $defender_archer,
        $defender_infantry, $defender_cavalry, $defender_pikemen,
        $defender_siege,    $draws,            $attacker_wins,
        $defender_wins,     $avg_rounds
    ) = @_;

    my $self = {
        num_simulations   => $num_simulations,
        general_roll      => $general_roll,
        risk_europe       => $risk_europe,
        risk              => $risk,
        verbose           => $verbose,
        stats             => $stats,
        attacker_peasent  => $attacker_peasent,
        attacker_archer   => $attacker_archer,
        attacker_infantry => $attacker_infantry,
        attacker_cavalry  => $attacker_cavalry,
        attacker_pikemen  => $attacker_pikemen,
        attacker_siege    => $attacker_siege,
        defender_peasent  => $defender_peasent,
        defender_archer   => $defender_archer,
        defender_infantry => $defender_infantry,
        defender_cavalry  => $defender_cavalry,
        defender_pikemen  => $defender_pikemen,
        defender_siege    => $defender_siege,
        draws             => $draws,
        attacker_wins     => $attacker_wins,
        defender_wins     => $defender_wins,
        avg_rounds        => $avg_rounds,
    };

    bless $self, $class;
    return $self;
}

sub pass_message {
    my ($desc) = @_;
    print color("green"), "PASS:", color("reset"), " $desc\n";
}

sub warn_message {
    my ($desc) = @_;
    print color("yellow"), "WARN:", color("reset"), " $desc\n";
}

sub fail_message {
    my ($desc) = @_;
    print color("red"), "FAIL:", color("reset"), " $desc\n";
}

sub run {
    my ($self) = @_;

    my $result = `./sim \\
        --num-simulations $self->{num_simulations} \\
        --general-roll $self->{general_roll} \\
        --risk-europe $self->{risk_europe} \\
        --risk $self->{risk} \\
        --verbose $self->{verbose} \\
        --stats $self->{stats} \\
        --attacker-peasent $self->{attacker_peasent} \\
        --attacker-archer $self->{attacker_archer} \\
        --attacker-infantry $self->{attacker_infantry} \\
        --attacker-cavalry $self->{attacker_cavalry} \\
        --attacker-pikemen $self->{attacker_pikemen} \\
        --attacker-siege $self->{attacker_siege} \\
        --defender-peasent $self->{defender_peasent} \\
        --defender-archer $self->{defender_archer} \\
        --defender-infantry $self->{defender_infantry} \\
        --defender-cavalry $self->{defender_cavalry} \\
        --defender-pikemen $self->{defender_pikemen} \\
        --defender-siege $self->{defender_siege} \\
    `;

    if (
        $result =~ /^After\ (?<num_simulations>\d+)\ simulations:\n
                     \twith\ an\ initial\ attacker\ value\ of\ (?<initial_attacker_value>\d+|n\/a)\n
                     \twith\ an\ initial\ defender\ value\ of\ (?<initial_defender_value>\d+|n\/a)\n
                     \tthere\ were\ (?<draws>\d+)\ draws\n
                     \tthere\ were\ (?<attacker_wins>\d+)\ attacker\ victories\n
                     \tthere\ were\ (?<defender_wins>\d+)\ defender\ victories\n
                     \tthere\ were\ an\ average\ of\ (?<avg_rounds>\d.+)\ rounds\n$/gmx
      )
    {
        # TODO make this work with regex
        if ( $self->{num_simulations} != $+{num_simulations} ) {
            fail_message("invalid number of simulations");
        }
        elsif ( $self->{draws} != $+{draws} ) {
            fail_message("invalid number of draws");
        }
        elsif ( $self->{attacker_wins} != $+{attacker_wins} ) {
            fail_message("invalid number of attacker wins $+{attacker_wins}");
        }
        elsif ( $self->{defender_wins} != $+{defender_wins} ) {
            fail_message("invalid number of defender wins $+{defender_wins}");
        }
        elsif ( $self->{avg_rounds} != $+{avg_rounds} ) {
            fail_message("invalid average rounds");
        }
        else {
            pass_message("pass");
        }
    }
    else {
        fail_message("simulation output failed match `$result`");
    }
}

1;
