# Milena Zlaticanin - Z1847022
# CSCI 330-1
# Assignment 7
# Due 11/06/20

BEGIN {
    FS = ":" # input field separator
    printf("%-20s\t%-15s\t%14s\n", "Name", "Position", "Sales Amount")

    # formatting
    for (x = 0; x < 54; x++)
	printf("=")
    
    printf("\n")

    products = 4
    associates = 3
    sales = 5
}

(NF == products) {
    pprice[$1] = $4 # field for products price(floating point number with 2 significant digits)
}

(NF == associates) {
    # splitting the name up, so the last name comes first, and then "," first name 
    split($2, name, " ")
    aname[$1] = name[2] ", " name[1]
    aposition[$1] = $3 # field for associates position(a string describing the job position of the associate)
    atotal[$1] = 0.0 # total sales
}

(NF == sales) {
    sdate[$1] = $4 # field for sales date(date of the transaction in the format mm/dd/yyyy)
    
    atotal[$5] = atotal[$5] + pprice[$2] * $3 # adding up the total sales
}

END {
    # going through each array and printing it to a |
    for (y in atotal)
	printf("%-20s\t%-15s\t%14.2f\n", aname[y], aposition[y], atotal[y]) | "sort -nr -k 4"
}
