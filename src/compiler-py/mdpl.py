import sys
import os

from tokenise import *
from passer import *
from transpiler import *

def setColorRed():
	print("\x1b[31m", end="")
def setColorGreen():
	print("\x1b[32m", end="")
def setColorYellow():
	print("\x1b[33m", end="")
def setColorBlue():
	print("\x1b[34m", end="")
def setColorMagenta():
	print("\x1b[35m", end="")
def setColorCyan():
	print("\x1b[36m", end="")
def setColorGrey():
	print("\x1b[37m", end="")
def setColorWhite():
	print("\x1b[38m", end="")
def resetColor():
	print("\x1b[39m", end="")
def underlineOn():
	print("\x1b[4m", end="")
def underlineOff():
	print("\x1b[24m", end="")

class Token:
	def __init__(self):
		self.str = None
		self.lineNum = None
		self.category = None

def main():
	if len(sys.argv) == 1:
		print("Error did not provide file to compile")
		return
	if len(sys.argv) >= 3:
		print("Error provided too many arguments")
		return
	fileName = sys.argv[1]
	if not os.path.exists(fileName):
		print("Error file does not exist")
		return
	if not os.access(fileName, os.R_OK):
		print("Error you do not have permission to open file")
		return
		
	
	tokens = tokenise(fileName)
	statments = groupStatments(tokens)
	scopes = groupScopes(tokens)
	categorise(tokens)
	transpile(fileName, tokens, statments, scopes)
	
	print()
	print("================================ Lines ================================")
	print()
	
	pLineNum = 1
	pStatmentIndex = 0
	print(str(tokens[0].lineNum) + ".  ", end="")
	underlineOn()
	for i,token in enumerate(tokens):
		if token.lineNum != pLineNum:
			if token.statmentIndex != pStatmentIndex:
				underlineOff()
			print(f"\n{str(token.lineNum)+".":<4}", end="")
			if token.statmentIndex != pStatmentIndex:
				underlineOn()
			pLineNum = token.lineNum
		
		if token.statmentIndex != pStatmentIndex:
			pStatmentIndex = token.statmentIndex
			underlineOff()
			if i != 0:
				print(" ", end="")
			if token.statmentIndex != None:
				underlineOn()

		else:
			print(" ", end="")	
		
		if token.category == enums.categories["Empty"]:
			setColorRed()
		elif token.category == enums.categories["Symbol"]:
			setColorGreen()
		elif token.category == enums.categories["Keyword"]:
			setColorYellow()
		elif token.category == enums.categories["StringLiteral"]:
			setColorBlue()
		elif token.category == enums.categories["FunctionName"]:
			setColorMagenta()
		elif token.category == enums.categories["Type"]:
			setColorCyan()
		elif token.category == enums.categories["Variable"]:
			setColorGrey()
		elif token.category == enums.categories["Number"]:
			setColorWhite()
		print(token.str.strip(), end="")
		resetColor()
	underlineOff()
	print()
	print()
	print("================================ Statments ================================")
	print()
	for statment in statments:
		print(f"{statment.startTokenIndex}-{statment.endTokenIndex}: ", end="")
		for i in range(statment.startTokenIndex, statment.endTokenIndex + 1):
			if tokens[i].category == enums.categories["Empty"]:
				setColorRed()
			elif tokens[i].category == enums.categories["Symbol"]:
				setColorGreen()
			elif tokens[i].category == enums.categories["Keyword"]:
				setColorYellow()
			elif tokens[i].category == enums.categories["StringLiteral"]:
				setColorBlue()
			elif tokens[i].category == enums.categories["FunctionName"]:
				setColorMagenta()
			elif tokens[i].category == enums.categories["Type"]:
				setColorCyan()
			elif tokens[i].category == enums.categories["Variable"]:
				setColorGrey()
			elif tokens[i].category == enums.categories["Number"]:
				setColorWhite()
			print(tokens[i].str + " ", end="")
			resetColor()
		print()
	print()
	print("================================ Scopes ================================")
	print()
	for scope in scopes:
		print(f"{scope.startTokenIndex}-{scope.endTokenIndex}: ", end="")
		for i in range(scope.startTokenIndex, scope.endTokenIndex + 1):
			if tokens[i].category == enums.categories["Empty"]:
				setColorRed()
			elif tokens[i].category == enums.categories["Symbol"]:
				setColorGreen()
			elif tokens[i].category == enums.categories["Keyword"]:
				setColorYellow()
			elif tokens[i].category == enums.categories["StringLiteral"]:
				setColorBlue()
			elif tokens[i].category == enums.categories["FunctionName"]:
				setColorMagenta()
			elif tokens[i].category == enums.categories["Type"]:
				setColorCyan()
			elif tokens[i].category == enums.categories["Variable"]:
				setColorGrey()
			elif tokens[i].category == enums.categories["Number"]:
				setColorWhite()
			print(tokens[i].str + " ", end="")
			resetColor()
		print()
	print()
	

if __name__ == "__main__":
	main()
