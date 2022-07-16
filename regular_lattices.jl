### A Pluto.jl notebook ###
# v0.17.5

using Markdown
using InteractiveUtils

# ╔═╡ afc784dc-07ca-4077-855c-de5054b817fc
md"""
# Lattices                          

define in main.jl:
                               				  				                                       		
		G  				#group size				  						 
		N				#population size			 					  
		L				#linear population size		 					  
		viz[N,G] 		#neighbors locations

for the boundary conditions to work N must have range 0:N-1	inside viz;
one way of achieving this is to use

	import Pkg 
	Pkg.add("OffsetArrays")
	using OffsetArrays

	viz = OffsetArray{Int64}(undef, 0:N-1, 0:G-1)

"""



# ╔═╡ 6075bafc-0c4e-4f60-8a5e-c05510ffae01
md"""
# 1D lattice
"""

# ╔═╡ 55c26a40-074a-4fb9-89e3-098c002966ba
function unidimensional_lattice(L)

	N = L
	for x = 0:N-1	
		viz[x,0] = x 		# self-interaction
		viz[x,1] = x - 1 	#left neighbor
		viz[x,2] = x + 1 	#right neighbor
		
		if(x == 0) viz[x,1] = L - 1 #left boundary
		end
		if(x == L - 1) viz[x,2] = 0 #right boundary
		end
	end
	
end


# ╔═╡ f8cb9c6d-50d0-4e56-a1a5-5c7d4ce688a5
md"""
# Square von Neumann 
"""

# ╔═╡ f678a49a-b926-4494-853a-bd6ad0da1690
function square_lattice(L)

	L2 = L*L
	N  = L*L

	for x = 0:N-1	
	
		viz[x,5] = x 		#self-interaction
		viz[x,1] = x - L 	#top neighbor
		viz[x,2] = x + 1 	#right neighbor
		viz[x,3] = x + L 	#botton neighbor
		viz[x,4] = x - 1 	#left neighbor
				
		if(x < L) viz[x,1] = x + (L - 1) * L 			#top boundary
		end
		if(x % L == 0) viz[x,4] = x + (L - 1) 			#left boundary
		end
		if(x >= L2-L) viz[x,3] = x - (L - 1) * L 		#botton boundary
		end
		if((x-L+1) % L == 0) viz[x,2] = x - (L - 1) 	#right boundary
		end
	end

end

# ╔═╡ 0a072586-b2bf-4ae9-9839-2fc8a194265f
md"""
# Cubic lattice
"""

# ╔═╡ 94effb8a-8620-4468-94f0-e7bcfa9b0d8c
function cubic_lattice(L)

	L2 = L*L
	L3 = L*L*L
	N = L*L*L;
	
	for x = 0:N-1	
		
		viz[x,0] = x 		#self-interaction
		viz[x,1] = x - L 	#top neighbor
		viz[x,2] = x + 1 	#right neighbor
		viz[x,3] = x + L 	#botton neighbor
		viz[x,4] = x - 1 	#left neighbor
				
		viz[x,5] = x + L2 	#back neighbor
		viz[x,6] = x - L2 	#front neighbor

		if(x % (L2) < L) viz[x,1] = x + (L - 1) * L 		#top boundary
		end
		if(x % L == 0) viz[x,4] = x + (L - 1)				#left boundary
		end
		if(x % (L2) >= L2-L) viz[x,3] = x - (L - 1) * L 	#botton boundary
		end
		if((x-L+1) % L == 0) viz[x,2] = x - (L - 1) 		#right boundary
		end
		if(x < L2) viz[x,6] = x + L3 - L2 					#front boundary
		end
		if(x >= L3 - L2) viz[x,5] = x - L3 + L2 			#back boundary
		end
	end

end

# ╔═╡ 0e3bcbf5-9ae2-43c1-b735-8cf615df603f
md"""
# 4D hypercubic
"""

# ╔═╡ 156e6ed0-5f2c-42ad-8c6d-ec44418da1ba
function hypercubic_lattice(L)

	L2 = L*L
	L3 = L*L*L
	L4 = L*L*L*L
	N = L*L*L*L

	for x = 0:N-1		
		
		viz[x,0] = x 		# self-interaction
		viz[x,1] = x - L 	#top neighbor
		viz[x,2] = x + 1 	#right neighbor
		viz[x,3] = x + L 	#botton neibhor
		viz[x,4] = x - 1 	#left neighbor
				
		viz[x,5] = x + L2 	#back neighbor
		viz[x,6] = x - L2 	#front neighbor

		viz[x,7] = x + L3 	#front front neighbor
		viz[x,8] = x - L3 	#back back neighbor

		if(x % (L2) < L) viz[x,1] = x + (L - 1) * L 		#top boundary
		end
		if(x % L == 0) viz[x,4] = x + (L - 1)				#left boundary
		end
		if(x % (L2) >= L2-L) viz[x,3] = x - (L - 1) * L 	#botton boundary
		end
		if((x-L+1) % L == 0) viz[x,2] = x - (L - 1) 		#right boundary
		end
		if(x % (L3) < L2) viz[x,6] = x + L3 - L2 			#front boundary
		end
		if(x % (L3) >= L3 - L2) viz[x,5] = x - L3 + L2 	#back boundary
		end
		if(x < L3) viz[x,8] = x + L4 - L3 					#front front boundary
		end
		if(x >= L4 - L3) viz[x,7] = x % (L3) 				#back back boundary
		end

	end
end


# ╔═╡ 54926377-c10f-4003-b9a2-fafac8fad806
md"""
# kagome lattice
"""

# ╔═╡ 09861767-4049-43ab-bf03-d4af29f3d70c
function kagome_lattice(L)

	L2 = L*L
	N  = 3*L*L;
	
	for x = 0:N-1		
	
		if(x % 3 == 0)
			viz[x,0] = x				# self-interaction
			viz[x,1] = x + 1 			#right neighbor
			viz[x,2] = x - 2 			#left neighbor
			viz[x,3] = x + 2 			#right neighbor
			viz[x,4] = x - 3*L + 2 	#botton neighbor
		end
		if(x % 3 == 1) 
			viz[x,0] = x 				# self-interaction
			viz[x,1] = x - 1 			#left neighbor
			viz[x,2] = x + 1 			#right neighbor
			viz[x,3] = x + 2 			#right neighbor
			viz[x,4] = x - 3*L + 4 	#botton neighbor				
		end
		if(x % 3 == 2) 
			viz[x,0] = x 				# self-interaction
			viz[x,1] = x - 1 			#left neighbor
			viz[x,2] = x - 2 			#right neighbor
			viz[x,3] = x + 3*L - 2 	#right neighbor
			viz[x,4] = x + 3*L - 4 	#botton neighbor			
		end

		if((x+2) % (3*L) == 0 && x % 3 == 1) #right boundary
			viz[x,3] = x - 3*L + 2
			viz[x,4] = x - 6*L + 4
		end
		
		if((x) % (3*L) == 0 && x % 3 == 0) 	#left boundary
			viz[x,2] = x + 3*L - 2
		end
		
		if((x+3*L-2) % (3*L) == 0 && x % 3 == 2) #left boundary
			viz[x,4] = x + (3*L - 2) * 2 
		end
		
		if(x > 3*L2 - 3*L && x % 3 == 2) #top boudnary		
			viz[x,3] = x - 3*L2 + 3*L - 4
			viz[x,4] = x - 3*L2 + 3*L - 2
		end	
		
		if(x < 3*L) #botton boundary		
			if(x % 3 == 0) viz[x,4] = x + 3*L2 - 3*L + 2 
			end
			if(x % 3 == 1) viz[x,4] = x + 3*L2 - 3*L + 4
			end
		end
		
		if(x == 3*L2 - 3*L + 2) #vertice
			viz[x,4] = 3*L - 2
		 	viz[x,3] = 0
		end 
		
		if(x == 3*L - 2) #vertice
			viz[x,4] = 3*L2 - 3*L + 2
		end
	end
end


# ╔═╡ f7217320-e17a-485f-9546-aa6cec36a732
md"""
# honeycomb lattice
"""

# ╔═╡ 75a408fd-984a-47e5-9e30-f9272f6e1cc0
function honeycomb_lattice(L)

	L2 = L*L
	N  = 2*L2
	
	for x = 0:N-1	
		
		if(x % 2 == 0) #even site
			viz[x,0] = x 				# self-interaction
			viz[x,1] = x - 1 			#left neighbor
			viz[x,2] = x + 1 			#right neighbor
			viz[x,3] = x + 2 * L - 1 	#botton neighbor
			
		else #if (x % 2 != 0) #odd site
			viz[x,0] = x 				# self-interaction
			viz[x,1] = x - 1			#left neighbor
			viz[x,2] = x + 1 			#right neighbor
			viz[x,3] = x - 2 * L + 1 	#top neighbor				
		end

		if((x-2*L+1) % (2*L) == 0) #right boundary
			viz[x,2] = x - 2 * L + 1
		 	viz[x,3] = x - 4 * L + 1	
		end 
		
		if(x % (2*L) == 0 || x == 0) #left boundary
			viz[x,1] = x + 2 * L - 1
		 	viz[x,3] = x + 4 * L - 1 
		end
		
		if(x < 2*L && x % 2 != 0) #top boundary		
			viz[x,3] = x + 2 * L * (L-1) + 1
		end
		
		if(x > 2*L2 - 2*L - 1 && x % 2 == 0) #botton boundary
			viz[x,3] = x - 2 * L * (L-1) - 1
		end
		
		if(x == 2*L2 - 2*L) #left-botton vertice 
			viz[x,3] = 2*L - 1
		end
		
		if(x == 2*L - 1) #top-right vertice
			viz[x,3] = 2*L2 - 2*L
		end

	end
end


# ╔═╡ 90b3b4ec-9a54-4d65-aec4-857b572ae1b5
md"""
# triangular lattice
"""

# ╔═╡ baddb456-77f2-4724-91a8-db494079c9ea
function triangular_lattice(L)
	
	L2 = L*L;
	N  = L2;
	
	for x = 0:N-1	

		viz[x,0] = x 				# self-interaction
		viz[x,1] = x - L 			#top neighbor
		viz[x,2] = x + 1 			#right neighbor
		viz[x,3] = x + L			#botton neighbor
		viz[x,4] = x - 1 			#left neighbor
		viz[x,5] = x - L + 1 		#top-right neighbor
		viz[x,6] = x - 1 + L 		#botton-left neighbor


		if(x % (L2) < L) #top boundary
			viz[x,1] = x + (L - 1) * L
			viz[x,5] = x + (L - 1) * L + 1
		end
		
		if(x % L == 0) #left boundary
			viz[x,4] = x + (L - 1)
			viz[x,6] = x + (L - 1) + L
		end
		
		if(x % (L2) >= L2-L) #botton boundary
			viz[x,3] = x - (L - 1) * L
			viz[x,6] = x - (L - 1) * L - 1
		end
		
		if((x-L+1) % L == 0) #right boundary
			viz[x,2] = x - (L - 1)
			viz[x,5] = x - (L - 1) - L
		end
		
		if(x == L-1) #top-right vertice
			viz[x,5] = L * L - L 
		end
		
		if(x == L2-L) #botton-left vertice
			viz[x,6] = L - 1
		end
	end
end

# ╔═╡ c52f30d5-a835-4716-9de6-613999f44688
md"""
# square Moore
"""

# ╔═╡ 17e9c790-0521-11ed-0af0-278c270d5976
function moore_lattice(L)

	L2 = L*L
	N  = L2
	
	for x = 0:N-1	

		viz[x,0] = x 				# self-interaction
		viz[x,1] = x - L 			#top neighbor
		viz[x,2] = x + 1 			#right neighbor
		viz[x,3] = x + L 			#botton neighbor
		viz[x,4] = x - 1 			#left neighbor
		viz[x,5] = x - L + 1 		#top-right neighbor
		viz[x,6] = x - 1 + L 		#botton-left neighbor
		viz[x,7] = x - 1 - L 		#top-left neighbor
		viz[x,8] = x + 1 + L 		#botton-right neighbor
	
		if(x % (L2) < L) #top boundary
			viz[x,1] = x + (L - 1) * L
			viz[x,5] = x + (L - 1) * L + 1
			viz[x,7] = x + (L - 1) * L - 1
		end
		
		if(x % L == 0) #left boundary
			viz[x,4] = x + (L - 1)
			viz[x,6] = x + (L - 1) + L 
			viz[x,7] = x + (L - 1) - L
		end
		
		if(x % (L2) >= L2-L) #botton boundary
			viz[x,3] = x - (L - 1) * L
			viz[x,6] = x - (L - 1) * L - 1
			viz[x,8] = x - (L - 1) * L + 1
		end
		
		if((x-L+1) % L == 0) #right boundary
			viz[x,2] = x - (L - 1)
			viz[x,5] = x - (L - 1) - L
			viz[x,8] = x - (L - 1) + L
		end
		
		if(x == 0) #top-left vertice
			viz[x,7] = L * L - 1
		end
		
		if(x == L-1) #top-right vertice
			viz[x,5] = L * L - L 
		end
		
		if(x == L2-L) #botton-left vertice
			viz[x,6] = L - 1
		end
		
		if(x == L2-1) #botton-right vertice
			viz[x,8] = 0
		end

	end
end


# ╔═╡ Cell order:
# ╟─afc784dc-07ca-4077-855c-de5054b817fc
# ╟─6075bafc-0c4e-4f60-8a5e-c05510ffae01
# ╠═55c26a40-074a-4fb9-89e3-098c002966ba
# ╟─f8cb9c6d-50d0-4e56-a1a5-5c7d4ce688a5
# ╠═f678a49a-b926-4494-853a-bd6ad0da1690
# ╟─0a072586-b2bf-4ae9-9839-2fc8a194265f
# ╠═94effb8a-8620-4468-94f0-e7bcfa9b0d8c
# ╟─0e3bcbf5-9ae2-43c1-b735-8cf615df603f
# ╠═156e6ed0-5f2c-42ad-8c6d-ec44418da1ba
# ╟─54926377-c10f-4003-b9a2-fafac8fad806
# ╠═09861767-4049-43ab-bf03-d4af29f3d70c
# ╟─f7217320-e17a-485f-9546-aa6cec36a732
# ╠═75a408fd-984a-47e5-9e30-f9272f6e1cc0
# ╟─90b3b4ec-9a54-4d65-aec4-857b572ae1b5
# ╠═baddb456-77f2-4724-91a8-db494079c9ea
# ╟─c52f30d5-a835-4716-9de6-613999f44688
# ╠═17e9c790-0521-11ed-0af0-278c270d5976
