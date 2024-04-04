from pyDatalog import pyDatalog 


# Initial (binary) predicates (on which facts are built)
pyDatalog.create_terms('age, nbDependentChildren, yearlyIncome, currentAccount, savingsAccount, debtLevel, healthStatus, workStatus, loanPurpose, loanAmount, loanDuration, personalContribution, loanFile')

# Variables for the queries
pyDatalog.create_terms('X, Y, Z')


# Example of a set of facts
+age('p1', 35)
+nbDependentChildren('p1', 2)
+yearlyIncome('p1', 60000)
+currentAccount('p1', 10000)
+savingsAccount('p1', 30000)
+debtLevel('p1', 'low')
+healthStatus('p1', 'good')
+workStatus('p1', 'cdi')
+loanPurpose('p1', 'propertypurchase')
+loanAmount('p1', 400000)
+loanDuration('p1', 120)
+personalContribution('p1', 100000)
+loanFile('p1', 'complete')


# Abstraction rules 
pyDatalog.create_terms('ageRange, incomeRange, durationRange, contributionRange, amountRange, hasRegularSalary, hasSalary, savings')
 
ageRange(X, 'young')  <= age(X, Y) & (Y < 30)
ageRange(X, 'medium') <= age(X, Y) & (Y >= 30) & (Y < 50)
ageRange(X, 'senior') <= age(X, Y) & (Y >= 50)


print (ageRange (X, Y))
