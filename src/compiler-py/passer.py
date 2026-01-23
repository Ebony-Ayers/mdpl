import enums
from tokenise import *

def isFloat(s):
	try:
		float(s)
		return True
	except:
		return False

class Statment:
	def __init__(self):
		self.startTokenIndex = None
		self.endTokenIndex = None
		self.scopeIndex = None

class Scope:
	def __init__(self):
		self.startStatmentIndex = None
		self.startTokenIndex = None
		self.endStatmentIndex = None
		self.endTokenIndex = None

def categorise(tokens):
	#first pass
	for i in range(len(tokens)):
		token = tokens[i]
		
		if token.str == "":
			tokens[i].category = enums.categories["Empty"]

		elif token.str[0] in enums.symbolCharacters:
			tokens[i].category = enums.categories["Symbol"]
		
		elif token.str in enums.keywords:
			tokens[i].category = enums.categories["Keyword"]
		
		elif (token.str[0] == '\"') and (token.str[-1] == '\"'):
			tokens[i].category = enums.categories["StringLiteral"]
			
		elif (i > 0) and (tokens[i-1].str in ("fn", "sub")):
			tokens[i].category = enums.categories["FunctionName"]
			
		elif token.str in enums.defaultTypes:
			tokens[i].category = enums.categories["Type"]
			
		elif (i > 0) and (tokens[i-1].category == enums.categories["Type"]):
			tokens[i].category = enums.categories["Variable"]
		
		elif (i > 1) and (tokens[i-1].str == "ref") and (tokens[i-2].category == enums.categories["Type"]):
			tokens[i].category = enums.categories["Variable"]
			
		elif (i > 0) and (tokens[i-1].str == ":"):
			tokens[i].category = enums.categories["Variable"]
		
		elif (i > 0) and (tokens[i-1].str == "namespace"):
			tokens[i].category = enums.categories["Variable"]
		
		elif isFloat(token.str):
			tokens[i].category = enums.categories["Number"]
	
	#second pass
	for i in range(len(tokens)):
		token = tokens[i]
		
		if (i > 0) and (i < len(tokens) - 1) and (tokens[i-1].str == "prop") and (tokens[i+1].str == '='):
			tokens[i].category = enums.categories["Variable"]
		
		elif (i > 0) and (tokens[i-1].str == "prop") and (token.category == enums.categories["Uncatagorised"]):
			tokens[i].category = enums.categories["FunctionName"]
		
		elif (i < len(tokens) - 1) and (tokens[i].category == enums.categories["Uncatagorised"]) and (tokens[i+1].str in ('=', '<', '>', "==", "<=", ">=", "+=", "-=", "*=", "/=")):
			tokens[i].category = enums.categories["Variable"]
		
		elif token.category == enums.categories["Uncatagorised"]:
			tokens[i].category = enums.categories["Empty"]

def groupStatments(tokens):
	statments = []

	tokens[0].statmentIndex = 0
	
	statmentCounter = 0
	startTokenIndex = 0
	for i in range(1, len(tokens)):
		token = tokens[i]
		prevToken = tokens[i-1]
		
		if (token.lineNum != prevToken.lineNum) and (prevToken.str != ','):
			statmentCounter += 1
			statment = Statment()
			statment.startTokenIndex = startTokenIndex
			statment.endTokenIndex = i - 1
			statments.append(statment)
			startTokenIndex = i
		
		elif token.str == ';':
			statmentCounter += 1
			statment = Statment()
			statment.startTokenIndex = startTokenIndex
			statment.endTokenIndex = i - 1
			statments.append(statment)
			startTokenIndex = i + 1

		if token.str != ';':
			tokens[i].statmentIndex = statmentCounter
		else:
			tokens[i].statmentIndex = None
	
	return statments

def groupScopes(tokens):
	numOpenBraces = 0
	scopeStack = []
	scopes = []
	for i in range(len(tokens)):
		token = tokens[i]
		
		if token.str == "{":
			numOpenBraces += 1
			scope = Scope()
			scope.startStatmentIndex = token.statmentIndex
			scope.startTokenIndex = i
			scope.endStatmentIndex = None
			scope.endTokenIndex = None
			scopeStack.append(scope)
		elif token.str == "}":
			numOpenBraces -= 1
			scope = scopeStack.pop()
			scope.endStatmentIndex = token.statmentIndex
			scope.endTokenIndex = i
			scopes.append(scope)
	return scopes



