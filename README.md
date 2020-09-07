# DSP Programming with C++ - Hard Realtime Audio  

---

Farnam Adelkhani - Contact me at: Farnam7@gmail.com  

### Why use C++ over Python, JS, etc?  

- Most audio APIs use C or C++  
- C++ is faaast and runs "close to the metal"  
- Custom memory management  
- C++ provides the ability to write "real-time safe" code by supporting concurrency, atomic types, and lock-free programming  

### Two golden rules for DSP code:

1 - Never cause signal dropouts. - Every callback must be achieved under a specified amount of time  

2 - Don't do anything where you can't predict the time it will take. - Do not call anything that blocks/waits
... avoid the following:  

- std::mutex::lock()  

- while(! std::mutex::try_lock()) {...}  

- std::thread::join()  

  ... 2 Reasons why:  

  - If you're waiting for something you cant know how long your code will take... leading to dropouts.  
  - Priority inversion: audio thread is a high-priority system thread, you don't want to wait for a lower-priority thread.  

For **real-time** processing:  

- The algorithm must accept a new input sample, do the processing, then have the output sample(s) available before the next input arrives. This will prevent clicks, pops, glitches, and noise.  

---

### Conventions:  
**x(n)** is always the **input** sequence  

   - The variable n represents the location of the nth sample of the x-sequence.  
  **y(n)** is always the **output** sequence  
   - The variable n represents the location of the nth sample of the y-sequence.  
  **h(n)** is the **impulse response** of the algorithm  
   - A special sequence that represents the algorithm output for a single sample input or impulse.  
   - Good practice to use < assert > after each declaration to create failure points, useful for debugging.  

**4 common mistakes: ** 
1 - Don’t hold locks on the audio thread.  

- Like pthread_mutex_lock or @synchronized.    

2 - Don’t use Objective-C/Swift on the audio thread.  

- Like [myInstance doAThing] or myInstance.something.    

3 - Don’t allocate memory on the audio thread.  

- Like malloc(), or new Abcd or [MyClass alloc].    

4 - Don’t do file or network IO on the audio thread.  

- Like read, write, or sendto.    

  

---

**Why use a compressor?**  
Compression can be used to subtly massage a track to make it more natural sounding and intelligible without adding distortion, resulting in a song that's more “comfortable” to listen to. Additionally, many compressors — both hardware and software — will have a signature sound that can be used to inject wonderful coloration and tone into otherwise lifeless tracks. 

Alternately, over-compressing your music can really squeeze the life out of it. Having a good grasp of the basics will go a long way toward understanding how compression works, and confidently using it to your advantage.

**"Look-ahead" compression filtering:**  
Looks into the future, analyses the upcoming signal, and triggers the compressor to start compression earlier than the occurring transient.  
J/K: Compressor cannot actually look into the future...  
Instead, it allows a compressor to react to a signal earlier, by means of a side chain. Therefore, if a compressor is at its fastest attack and it still lets transients through, you can use the look-ahead to fix that.  

**How does this work?:**  
The look-ahead function duplicates the incoming signal, which is then delayed (by the amount that you set the lookahead value). The non-delayed signal is fed to a side-chain that drives the compression of the delayed signal. That signal is then outputted with the rest of the tracks. (The downside is that the output will be slightly delayed by the amount you set the lookahead)

**Why is this useful?:**  
 The look-ahead function is a great tool for dealing with transients. It allows you to theoretically have an attack that is “below 0 seconds”.  
 It is useful as it prevents an engineer from having to compromise between using a slower attack time (smoother gain changes) and faster attack rates which are capable of catching transients. So now you can get the best of both worlds.  

**What can this function help with?**  
- Brick-wall Limiting / Hard Limiting: Using look-ahead would prevent any peaks from passing through. It prevents transients from taking any headroom.  
- Mastering: Just as a final precaution when mastering you could use a lookahead to make sure that you don’t have any sound clipping the output. It also helps if you’re trying to participate in the loudness wars (Turn it up to 11!)  
- Punchier: This technique can be used to get some punch out of a track and bring out sustain in an element. When using parallel compression, you can use a look-ahead when compressing the parallel bus, which blends with the original track to achieve a punchier sound.  
- A slower attack with an early look-ahead time can allow you to save transients when applying compression. This way a smooth-sounding slower attack rate can be used to catch transients.

**Steps to creating a compressor with look-ahead functionality:**
1. Duplicate the track that you wish to affect.
2. Set the duplicated track to “No Output”
3. Nudge the duplicated track earlier (the earlier you nudge the track is equivalent to the higher you would set the look-ahead value)
4. Add a compressor on the original track (must have the ability to use a side-chain)
5. Create a send from the duplicate track and use it as the key on the compressor from the original track. (Note that on some compressors, you have to activate the key detector so that the compressor looks at that signal)
6. Adjust the Parameters

---

**4 Steps to implementing a look-ahead compressor (2 steps are the same as a regular compressor.)**

**Step 1:** Computation of the gain reduction

- Create a side-chain signal - for a limiter this should be the max over all channels to avoid missing any peaks.
- Send this side-chain signal to the >GainReductionComputer to get gain reduction out.
- It's better to have the attenuation in the logarithmic domain(in decibels) and without the makeup gain. There is a special method for this in GainReductionComputer.

**Step 2:** Delaying both input and gain reduction signals

- Important to give both signals the same delay! Apply gains right at transients.
- Delay-time parameter can be varied, 5ms is a good balance between latency & distortion.
- Delay value should be rounded to number of samples, so no need for a fractional delay.

**Step 3:** Smoothing your gain-reduction (Most important step)

- Smoothly fade-in our aggressive gain reduction values to avoid added distortions.
  - ie: When a transient is met, a gain-reduction is faded in.

**Step 4:** Apply the smooth gain-reduction

- Simply multiply the input signal with the smooth gain-reduction.