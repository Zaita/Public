red_hose = 8.5 * 60
combined = 5.25 * 60

# how much would red hose do in 5.25 hours
red_hose_partial = combined / red_hose * 100
print('Red Hose Partial: ' + str(red_hose_partial) + '%')

blue_hose_contribution = 100 - red_hose_partial
print('Blue Hose Contribution: ' + str(blue_hose_contribution))
print('--')

# How much per minute as a % would blue do
blue_hose_per_minute = blue_hose_contribution / combined
print('Blue Hose % Per Minute: ' + str(blue_hose_per_minute))

fill_minutes = 100 / blue_hose_per_minute
print('Number of Minutes: ' + str(fill_minutes))
hours = int(fill_minutes / 60)
minutes = int(round(fill_minutes - (hours * 60)))
print('Blue Time: ' + str(hours) + ' hours and ' + str(minutes) + ' minutes')

