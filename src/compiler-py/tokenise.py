import enums

class Token:
	def __init__(self):
		self.str = None
		self.lineNum = None
		self.category = enums.categories["Uncatagorised"]
		self.statmentIndex = None
		self.ScopeIndex = None

def tokenise(fileName):
	tokens = []
	lineCounter = 1
	tokenStart = 0
	inLineComment = False
	inBlockComment = False
	inStringLiteral = False
	
	with open(fileName, "r") as f:
		data = f.read()
		i = 0
		while i < len(data):
			c = data[i]
			
			if inLineComment:
				#exit condition for line comment
				if c == '\n':
					inLineComment = False
					tokenStart = i+1
				#keep track of lines even when skipping due to comment
				if c == '\n':
					lineCounter += 1
				i += 1
				continue
			if inBlockComment:
				#exit condition for block comment
				if (i+1 < len(data)) and (c == '*') and (data[i+1] == '/'):
					inBlockComment = False
					tokenStart = i+2
					i += 1
				#keep track of lines even when skipping due to comment
				if c == '\n':
					lineCounter += 1
				i += 1
				continue
			if inStringLiteral:
				if c == '\"':
					inStringLiteral = False
					token = Token()
					token.str = data[tokenStart:i+1].strip()
					token.lineNum = lineCounter
					tokens.append(token)
					tokenStart = i+1
				#keep track of lines even when skipping due to comment
				if c == '\n':
					lineCounter += 1
				i += 1
				continue
			
			#enter condition for line comment
			if (i+1 < len(data)) and (c == '/') and (data[i+1] == '/') and not inLineComment and not inBlockComment and not inStringLiteral:
				#a comment could end a token so create it
				if tokenStart != i:
					token = Token()
					token.str = data[tokenStart:i].strip()
					token.lineNum = lineCounter
					tokens.append(token)
				inLineComment = True;
				i += 2
				continue
			#enter condition for block comment
			if (i+1 < len(data)) and (c == '/') and (data[i+1] == '*') and not inLineComment and not inBlockComment and not inStringLiteral:
				#a comment could end a token so create it
				if tokenStart != i:
					token = Token()
					token.str = data[tokenStart:i].strip()
					token.lineNum = lineCounter
					tokens.append(token)
				inBlockComment = True;
				i += 2
				continue
			#enter condition for string literal
			if (c == '\"') and not inLineComment and not inBlockComment and not inStringLiteral:
				#a string literal could end a token so create it
				if tokenStart != i:
					token = Token()
					token.str = data[tokenStart:i].strip()
					token.lineNum = lineCounter
					tokens.append(token)
					#tokenStart = i
				inStringLiteral = True
				i += 1
				continue
			
			#check easy ways for a token to end
			if (c == ' ') or (c == '\n'):
				if tokenStart != i:
					token = Token()
					token.str = data[tokenStart:i].strip()
					token.lineNum = lineCounter
					tokens.append(token)
				tokenStart = i+1
			#we need to keep semi conons for statment creation
			if c == ';':
				if tokenStart != i:
					token = Token()
					token.str = data[tokenStart:i].strip()
					token.lineNum = lineCounter
					tokens.append(token)
				token = Token()
				token.str = c
				token.lineNum = lineCounter
				tokens.append(token)
				tokenStart = i+1
			#symbols have special cases so are check seperately
			elif c in enums.symbolCharacters:
				#a symbol may end a token so if this is the case create the token				
				if tokenStart != i:
					token = Token()
					token.str = data[tokenStart:i].strip()
					token.lineNum = lineCounter
					tokens.append(token)
				
				#add the symbol as a seperate token
				token = Token()
				#check for two character symbols e.g. ==
				if (i+1 < len(data)) and (data[i+1] in enums.symbolCharacters):
					token.str = data[i:i+2].strip()
					tokenStart = i+2
					i += 1
				else:
					token.str = data[i:i+1].strip()
					tokenStart = i+1
				token.lineNum = lineCounter
				tokens.append(token)
			
			if c == '\n':
				lineCounter += 1
			i += 1
	return tokens
