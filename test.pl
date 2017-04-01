#!/usr/bin/perl

use strict;
use warnings;

use TestCase;

my @tests = (

    # num_simulations, general_roll, risk_europe, risk, verbose, stats,
    # attacker: peasent, archer, infantry, cavalry, pikemen, siege,
    # defender: peasent, archer, infantry, cavalry, pikemen, siege,
    # draws, attacker wins, defense wins, avg_rounds
    new TestCase(
        1000, "true", "false", "true", "false", "false",
        1,    0,      0,       0,      0,       0,
        0,    0,      0,       0,      0,       0,

        0, 1000, 0, 1.00
    )
);

foreach my $test (@tests) {
    $test->run();
}
