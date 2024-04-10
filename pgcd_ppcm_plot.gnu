set style data histograms
set style fill solid
set boxwidth 0.5 relative
plot 'pgcd_ppcm.dat' using 2:xtic(1) lc rgb 'blue' title 'PPCM = 12 et PGCD = 2', '' using 0:1:2 with labels offset -0.5,-1 rotate by 90 tc lt 1
